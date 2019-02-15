package sample;

import javafx.animation.ScaleTransition;
import javafx.event.Event;
import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.effect.BoxBlur;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.DragEvent;
import javafx.scene.input.TransferMode;
import javafx.scene.layout.VBox;
import javafx.util.Duration;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.List;

public class DemoController {
    @FXML
    private Button accountBTN;
    @FXML
    private ImageView image_holder;
    @FXML
    private Button hashBTN;
    @FXML
    private VBox vBox;

    private static final String defaultImage = "file:src/drag-drop-upload-2.gif";

    DemoController(){

    }

    @FXML
    private void initialize(){
        image_holder.setImage(new Image(defaultImage));
        imageScaleUp();
    }

    @FXML
    private void handleAccountClick(Event event){

    }
    @FXML
    private void handleDragOver(DragEvent event){
        if(event.getDragboard().hasFiles()){
            event.acceptTransferModes(TransferMode.ANY);
        }

    }
    @FXML
    private void handleDrop (DragEvent event) throws FileNotFoundException {
        image_holder.setEffect(new BoxBlur());
        List<File> files = event.getDragboard().getFiles();
        Image img = new Image(new FileInputStream(files.get(0)));
        image_holder.setImage(img);
        imageScaleUp();
        hashBTN.setVisible(true);
    }

    @FXML
    private void handleHash(Event event) throws InterruptedException {
        image_holder.setImage(new Image("file:src/barcode.gif"));
        imageScaleUp();

        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setContentText("Your Image has been hashed and sent to Facebook");
        alert.setHeaderText("Image Hashed");
        alert.setTitle("Information");
        alert.showAndWait();

        image_holder.setEffect(null);
        image_holder.setImage(new Image(defaultImage));
        imageScaleUp();
        hashBTN.setVisible(false);
    }

    private void imageScaleDown(){
        //suppose to zoom scale the image down but doesnt work correctly
        ScaleTransition scaleTransition = new ScaleTransition(Duration.millis(3000), image_holder);
        scaleTransition.setFromX(1);
        scaleTransition.setFromY(1);
        scaleTransition.setByX(0);
        scaleTransition.setByY(0);
        scaleTransition.play();
    }
    private void imageScaleUp(){
        ScaleTransition scaleTransition = new ScaleTransition(Duration.millis(1000), image_holder);
        scaleTransition.setFromX(0);
        scaleTransition.setFromY(0);
        scaleTransition.setByX(1);
        scaleTransition.setByY(1);
        scaleTransition.play();
    }


}
