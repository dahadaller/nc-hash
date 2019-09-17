package com.nchash.view;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;

import java.io.IOException;

public class Main extends Application {

    private Stage primaryStage;
    private BorderPane rootLayout;

    /**
     * Constructor
     */
    public Main() {
    }

    /**
     * Initializes the root layout.
     */
    public void initRootLayout() {
        try {
            // Load root layout from fxml file.
            FXMLLoader loader = new FXMLLoader();
            loader.setLocation(Main.class.getResource("RootLayout.fxml"));
            rootLayout = (BorderPane) loader.load();

            // Show the scene containing the root layout.
            Scene scene = new Scene(rootLayout);
            primaryStage.setScene(scene);
            primaryStage.show();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

//    public void showMainApplication() {
//        try {
//            FXMLLoader loader = new FXMLLoader();
//            loader.setLocation(Main.class.getResource("CenterView.fxml"));
////            CenterViewController centerViewController = new CenterViewController();
//            loader.setController(centerViewController);
//            Scene scene = new Scene(loader.load());
//            primaryStage.setScene(scene);
//            primaryStage.setTitle("NC-Hash Application");
//            primaryStage.show();
//        } catch (Exception e) {
//            e.printStackTrace();
//        }
//    }

    public void showMainApplication() {
        try {

            FXMLLoader loader = new FXMLLoader();
            loader.setLocation(Main.class.getResource("CenterView.fxml"));
            BorderPane centerView = (BorderPane) loader.load();

            rootLayout.setCenter(centerView);

            CenterViewController centerViewController = loader.getController();
            centerViewController.setMainApp(this);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

//    public void showPersonOverview() {
//        try {
//            // Load person overview.
//            FXMLLoader loader = new FXMLLoader();
//            loader.setLocation(MainApp.class.getResource("view/PersonOverview.fxml"));
//            AnchorPane personOverview = (AnchorPane) loader.load();
//
//            // Set person overview into the center of root layout.
//            rootLayout.setCenter(personOverview);
//
//            // Give the controller access to the main app.
//            PersonOverviewController controller = loader.getController();
//            controller.setMainApp(this);
//
//        } catch (IOException e) {
//            e.printStackTrace();
//        }
//    }

    //    @Override
//    public void start(Stage primaryStage) throws Exception{
//        Parent root = FXMLLoader.load(getClass().getResource("sample.fxml"));
//        primaryStage.setTitle("Hello World");
//        primaryStage.setScene(new Scene(root, 300, 275));
//        primaryStage.show();
//    }
    @Override
    public void start(Stage primaryStage) {
        this.primaryStage = primaryStage;
        this.primaryStage.setTitle("AddressApp");

        initRootLayout();
        showMainApplication();

//        showPersonOverview();
    }

    /**
     * Returns the main stage.
     *
     * @return
     */
    public Stage getPrimaryStage() {
        return primaryStage;
    }


    public static void main(String[] args) {
        launch(args);
    }
}
