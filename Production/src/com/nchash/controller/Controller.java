package com.nchash.controller;

import javafx.fxml.Initializable;
import javafx.scene.control.Button;

import java.net.URL;
import java.util.ResourceBundle;

public class Controller implements Initializable {



    public void loginButtonClicked() {
        System.out.println("User logged in...");
    }

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        System.out.println("Loading use data...");
    }
}
