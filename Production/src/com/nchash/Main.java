package com.nchash;

import com.nchash.controller.MainController;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;

import javafx.stage.Stage;

public class Main extends Application {

    public static void main(String[] args) {
        launch(args);
    }

    private Stage primaryStage;

    @Override
    public void start(Stage primaryStage) throws Exception{
        this.primaryStage = primaryStage;
        primaryStage.setHeight(500);
        primaryStage.setWidth(700);

        showMainApplication();
//
//        Scene scene = new Scene(createContent());
//        primaryStage.setScene(scene);
//        primaryStage.show();
    }

    public void showMainApplication(){
        try{
            FXMLLoader loader = new FXMLLoader();
            loader.setLocation(Main.class.getResource("view/MainView.fxml"));
            MainController mainController = new MainController();
            loader.setController(mainController);
            Scene scene = new Scene(loader.load());
            primaryStage.setScene(scene);
            primaryStage.setTitle("NC-Hash Application");
            primaryStage.show();
        }catch (Exception e){
            e.printStackTrace();
        }
    }
//    Testing code for thanos effect on image. Ignore...... for now.
//    private GraphicsContext g;
//    private Parent createContent(){
//        Pane root = new Pane();
//        Canvas canvas = new Canvas(300, 500);
//        root.getChildren().add(canvas);
//        g = canvas.getGraphicsContext2D();
//        Image image = new Image("file:src/com/nchash/images/armor.jpg");
//        return root;
//    }
    public Stage getPrimaryStage() {
        return primaryStage;
    }
}
