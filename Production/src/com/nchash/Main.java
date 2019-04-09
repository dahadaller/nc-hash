package com.nchash;

import com.nchash.controller.Controller;
import javafx.application.Application;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;

public class Main extends Application {

    public static void main(String[] args) {
        launch(args);
    }

    private Stage primaryStage;

    @Override
    public void start(Stage primaryStage) throws Exception{
        this.primaryStage = primaryStage;
        showDebugScreen();
    }

    /**
     * Shows Bon's code which is the debug window
     */
    public void showDebugScreen(){
        try{
            FXMLLoader loader = new FXMLLoader();
            loader.setLocation(Main.class.getResource("view/sample.fxml"));
            BorderPane debugScreenView = (BorderPane)loader.load();

            Scene scene = new Scene(debugScreenView);
            Controller controller = loader.getController();
            controller.setDebugStage(primaryStage);
            primaryStage.setScene(scene);
            primaryStage.show();
        }catch (Exception e){
            e.printStackTrace();
        }

    }




}
