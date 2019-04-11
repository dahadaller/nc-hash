package com.nchash.controller;

import com.nchash.Main;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.stage.Stage;

import java.net.URL;
import java.util.ResourceBundle;

public class Controller implements Initializable {

    private Stage debugStage;
    private Main mainApp;

    public void loginButtonClicked() {
        System.out.println("User logged in...");
        //Test to see if primary stage aka debugStage closes.
        debugStage.close();
    }


    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        System.out.println("Loading use data...");
    }


    public void setMainApp(Main mainApp) {
        this.mainApp = mainApp;
        this.debugStage = mainApp.getPrimaryStage();
    }
}
