package sample;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
//import javafx.scene.control.Button;
//import javafx.scene.layout.StackPane;
import javafx.stage.Stage;

public class Main extends Application {

//    Button button;

    @Override
    public void start(Stage primaryStage) throws Exception {
        Parent root = FXMLLoader.load(getClass().getResource("sample.fxml"));
        primaryStage.setTitle("Hello World");

//        button = new Button();
//        button.setText("Click me");

//        StackPane layout = new StackPane();
//        layout.getChildren().add(button);

//        Scene scene = new Scene(layout, 300, 275);

        primaryStage.setScene(new Scene(root, 1000, 500));
//        primaryStage.setScene(scene);
        primaryStage.show();
    }


    public static void main(String[] args) {
        launch(args);
    }
}
