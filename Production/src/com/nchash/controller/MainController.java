package com.nchash.controller;

import com.nchash.model.Particle;
import com.nchash.view.CppHook;
import javafx.animation.AnimationTimer;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.geometry.Point2D;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.image.PixelReader;
import javafx.scene.input.DragEvent;
import javafx.scene.input.TransferMode;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.stage.FileChooser;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;

/**
 * This is the main controller. It contains all the functionality
 * need for the application to fun. It contains the view and the
 * event functions.
 */
public class MainController {
    private double height = 300.0;
    private double width  = 500.0;

    private final static String defaultUploadImage = "file:src/com/nchash/images/drag_and_drop_image.jpg";
    private Image uploadImage = new Image(defaultUploadImage, width, height, true, false);
    private Image mainImage;

    private GraphicsContext g;
    private List<Particle> particleList = new ArrayList<>();

    private Button hashButton = new Button("Hash");

    @FXML
    private VBox centerVBox;
    @FXML
    private Canvas mainCanvas;
    @FXML
    private Label pathToFileLabel;

    /**
     * Initializes the controller class. This method is automatically called
     * after the fxml file has been loaded.
     */
    @FXML
    public void initialize(){
        g = mainCanvas.getGraphicsContext2D();
        setCurrentImage(uploadImage);

        hashButton.setOnAction(getHashButtonEventHandler());

    }

    /**
     * Handles the hash button function. It calls the disintegrate
     * function for the image and removes the hash button.
     * Creates a new Animation timer, this timer will call update() every
     * frame until all the pixels are dead, then the timer will call the
     * stop() function. It then resets the global alpha to 1.0 to allow for
     * a new image to be visible for the user. It then sets the uploadImage
     * indicating to the user that another image can be drag and dropped for hashing.
     * @return
     */
    private EventHandler getHashButtonEventHandler(){
        return new EventHandler() {
            @Override
            public void handle(Event event) {
                disintegrate(mainImage);
                centerVBox.getChildren().remove(hashButton);
                AnimationTimer timer = new AnimationTimer() {
                    @Override
                    public void handle(long now) {
                        update();
                        if (particleList.isEmpty()){
                            this.stop();
                            g.setGlobalAlpha(1.0);
                            setCurrentImage(uploadImage);
                        }
                    }
                };
                timer.start();
            }
        };
    }

    /**
     * Gets the pixel reader of the image. It then reads each pixel
     * and saves that x, y, and color values inside a particle object.
     * Each particle is then added to the particle list.
     * @param image
     */
    private void disintegrate(Image image){
        PixelReader pixelReader = image.getPixelReader();

        for (int y = 0 ; y < image.getHeight(); y++){
            for (int x = 0; x < image.getWidth(); x++){
                Color color = pixelReader.getColor(x, y);
                //Ignores transparent pixels.
                if (!color.equals(Color.TRANSPARENT)){
                    Particle p = new Particle(x, y, color);
                    particleList.add(p);
                }
            }
        }
    }

    /**
     * This function is called each frame.
     *
     * Removes dead particles, so that we won't need to look at it anymore.
     * Checks if the particle is active and if it is active, we will call the method activate
     * on the particle. This gives the particle a new velocity. This velocity is a random
     * double type number greater than or equal to 0.0 and less than 1.0.
     *
     * It then calls the method update on each particle to slowly decrease the life member variable (alpha).
     * It then calls the draw method for the particle and passes in the graphical context for reference.
     */
    private void update(){
        g.clearRect(0,0, width , height);
        particleList.removeIf(Particle::isDead);

        particleList.parallelStream()
                .filter(particle -> !particle.isActive())
                .forEach(particle -> particle.activate(new Point2D(- Math.random()  ,  - Math.random()  )));

        particleList.forEach(particle -> {
            particle.update();
            particle.draw(g);
        });

    }

    /**
     * Checks if every pixel in the particle list is dead.
     * Function isn't used, but I may need it later.
     * @return boolean
     */
    private boolean isImageDead(){
        for (Particle particle : particleList){
            if (!particle.isDead()){
                return false;
            }
        }
        return true;
    }



    /**
     * This function opens the file explorer window. The file selected must be an image.
     * Once the image is selected the default image will be set to the selected image.
     */
    public void handleUploadBtn(){
        String pathToFile = openFileExplorer();
        if(pathToFile != null){

            CppHook cpp = new CppHook();
            System.out.println("CppHook Declaration Successful.");
            cpp.printMsg("YAYYY NATIVE CODE!!!!"); //TODO: DON"T FORGET THIS
            System.out.println(cpp.please_work("This message passes through C++"));

            if (!centerVBox.getChildren().contains(hashButton)){
                centerVBox.getChildren().add(hashButton);
            }
            mainImage = new Image(pathToFile, width, height, true, false);
            setCurrentImage(mainImage);
        }
    }


    /**
     * This function will open the file explorer. The user will pick a file
     * and the path of the file will be returned else it'll returns null.
     * @return path to file String. Null if no file is selected.
     */
    private String openFileExplorer(){
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("File Explorer");
        File file = fileChooser.showOpenDialog(null);
        if(isImageFile(file) && file != null){
            StringBuilder pathToFile = new StringBuilder("file:");
            pathToFile.append(file.getAbsolutePath());
            return pathToFile.toString();
        }
        return null;
    }

    /**
     * This function will handle the drag over file feature.
     * If the file is accepted the handle drop function will be called else
     * the previous image will remain.
     * @param event
     */
    @FXML
    private void handleDragOver(DragEvent event){
        if(event.getDragboard().hasFiles()){
            event.acceptTransferModes(TransferMode.ANY);
        }
    }

    /**
     * Once the image is dropped into the window. It checks to see if the
     * hash button is a child of the vBox, if not; it adds it.
     * The mainimage is now equal to what was dropped into the window and the image
     * will be displayed.
     * @param event
     * @return A string containing the path to file.
     * @throws FileNotFoundException
     */
    @FXML
    private void handleDrop(DragEvent event){
        try {
            if (!centerVBox.getChildren().contains(hashButton)){
                centerVBox.getChildren().add(hashButton);
            }
            String pathToFile = getPathToImageDragAndDrop(event);
            if (pathToFile != null){
                mainImage = new Image(getPathToImageDragAndDrop(event), width, height, true, false);
                setCurrentImage(mainImage);

            }else {
                setCurrentImage(null);
                centerVBox.getChildren().remove(hashButton);

            }
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    /**
     * This function takes in an DragEvent object and returns a String
     * containing the path to the image. As of right now it only returns 1 file.
     * The function can be changed to return multiple files in the DragEvent.
     * @param event
     * @return
     */
    private String getPathToImageDragAndDrop(DragEvent event){
        StringBuilder pathToImage = new StringBuilder("File:");
        File file = event.getDragboard().getFiles().get(0);
        if (isImageFile(file)){
            pathToImage.append(event.getDragboard().getFiles().get(0));
            event.consume();
            return pathToImage.toString();
        }
        return null;

    }

    /**
     * Sets the main image in application to any image.
     *
     * If the image is null, it simply returns.
     * Before it draws the image for the user to see. It clears the canvas giving
     * us a blank canvas to work with. Once the canvas is blank it will draw the image
     * for the user to see.
     * @param image
     */
    private void setCurrentImage(Image image){
        if (image == null){
            return;
        }
        g.clearRect(0,0, width , height);
        g.drawImage(image, 0,0, image.getWidth(), image.getHeight());
        pathToFileLabel.setText(image.getUrl());
    }

    /**
     * Checks if the file is an image.
     * @param file
     * @return true if file is an image, false if not.
     */
    private boolean isImageFile(File file){
        String filePath = file.getAbsolutePath().toLowerCase();
        return  filePath.endsWith(".png") ||
                filePath.endsWith(".img") ||
                filePath.endsWith(".jpg");
    }

}
