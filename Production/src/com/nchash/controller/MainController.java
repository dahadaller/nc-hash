package com.nchash.controller;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.DragEvent;
import javafx.scene.input.TransferMode;
import javafx.stage.FileChooser;

import java.io.File;
import java.io.FileNotFoundException;


public class MainController {
    private final static String defaultUploadImage = "file:src/com/nchash/images/drag_and_drop_image.gif";

    @FXML
    private ImageView mainImageView;

    /**
     * Initializes the controller class. This method is automatically called
     * after the fxml file has been loaded.
     */
    @FXML
    public void initialize(){
       setDefaultUploadImage(defaultUploadImage);

    }

    /**
     * This function opens the file explorer window. The file selected must be an image
     */
    public void handleUploadBtn(ActionEvent event){
        String pathToFile = openFileExplorer();
        if(pathToFile != null){
            setDefaultUploadImage(pathToFile);
        }


    }


    /**
     * This function will open the file explorer. The user will pick a file
     * and the path of the file will be returned else returns null.
     * @return path to file String. Null if no file is selected.
     */
    private String openFileExplorer(){
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Open File Explorer");
        File imageFile = fileChooser.showOpenDialog(null);
        if(imageFile != null){
            StringBuilder pathToFile = new StringBuilder();
            pathToFile.append("file:");
            pathToFile.append(imageFile.getAbsolutePath());
            return pathToFile.toString();
        }
        return null;
    }

    /**
     * This function will handle the drag file feature.
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
     * Once the file has been accepted after the drag function is called.
     * This function will be called and returns a image.
     * @param event
     * @return A string containing the path to file.
     * @throws FileNotFoundException
     */
    @FXML
    private void handleDrop(DragEvent event){
        try {
            String pathToFile = getPathToImageDragAndDrop(event);
            System.out.println(pathToFile);
            setDefaultUploadImage(pathToFile);
        }catch (Exception e){
            e.getCause().printStackTrace();
        }
    }

    /**
     * This function takes in an DragEvent object and returns a String
     * containing the path to the image.
     * @param event
     * @return
     * @throws FileNotFoundException
     */
    private String getPathToImageDragAndDrop(DragEvent event)throws FileNotFoundException{
        StringBuilder pathToImage = new StringBuilder("File:");
        pathToImage.append(event.getDragboard().getFiles().get(0));
        return pathToImage.toString();
    }

    /**
     * Sets the main image in application to any image.
     * @param pathToFile
     */
    private void setDefaultUploadImage(String pathToFile){
        mainImageView.setImage(new Image(pathToFile));
    }

}
