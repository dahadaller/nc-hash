package com.nchash.controller;

import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.stage.Stage;

import java.net.URL;
import java.util.ResourceBundle;

public class Controller implements Initializable {

    private Stage debugStage;

    public void loginButtonClicked() {
        System.out.println("User logged in...");
        //Test to see if primary stage aka debugStage closes.
        debugStage.close();
    }

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        System.out.println("Loading use data...");
    }

    public void setDebugStage(Stage debugStage) {
        this.debugStage = debugStage;
    }
}
