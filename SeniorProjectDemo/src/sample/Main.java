package sample;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;
import java.io.IOException;
public class Main extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception {
        startDemo(primaryStage);
    }

    private void startDemo(Stage primaryStage) throws IOException {
        FXMLLoader loader = new FXMLLoader();

        loader.setController(new DemoController());

        loader.setLocation(Main.class.getResource("demo.fxml"));
        primaryStage.setTitle("Image Hashing Application");
        Scene scene = new Scene(loader.load(), 350,450);
        scene.getStylesheets().add(getClass().getResource("demo.css").toExternalForm());
        // scene.getStylesheets().add("org/kordamp/bootstrapfx/bootstrapfx.css");
        // trying to learn bootstrapfx.
        primaryStage.setScene(scene);

        primaryStage.show();
    }

}