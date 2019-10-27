package com.nchash.controller;

import javafx.scene.control.Alert;

public class GuiUtil {
    /**
     *
     * @param alertType
     *  An enum object from Alert class's AlertType enum array.
     * @param message
     *  Message to be display next to ok Button
     * @param header
     *  Text to be displayed on top of header and next to the AlertType logo
     * @param title
     *  title of newly create window.
     */
    public static void createAlertWindow(Alert.AlertType alertType, String message, String header, String title) {
        //postcondition: a new pop up window is shown with 'message' as body, 'header' as header and 'title' as title.
        Alert alert = new Alert(alertType);
        alert.setContentText(message);
        alert.setHeaderText(header);
        alert.setTitle(title);
        alert.showAndWait();
    }

}
