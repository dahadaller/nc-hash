package com.nchash.controller;

import javafx.fxml.FXML;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;


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
       mainImageView.setImage(new Image(defaultUploadImage));

    }

}
