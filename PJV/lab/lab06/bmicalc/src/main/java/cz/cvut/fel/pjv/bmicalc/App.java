package cz.cvut.fel.pjv.bmicalc;

import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.HBox;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;

public class App extends Application {


    private boolean isDouble(String s){
        try{
            Double.parseDouble(s);
            return true;
        }catch (NumberFormatException e){
            return false;
        }
    }

    private double calculateBMI(double weight, double height){
        return  weight / ((height / 100) * (height / 100));

    }

    @Override
    public void start(Stage stage) {

        TextField weightField = new TextField();
        weightField.setPromptText("Enter weight in kg");
        weightField.setPrefWidth(120);
        TextField heightField = new TextField();
        heightField.setPromptText("Enter height in cm");
        heightField.setPrefWidth(120);

        Button calcButton = new Button("Calculate");
        calcButton.setPrefWidth(80);

        Label result = new Label();
        result.setPrefWidth(120);

        calcButton.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent actionEvent) {

                if(isDouble(weightField.getText()) && isDouble(heightField.getText())){
                    result.setText(String.valueOf(calculateBMI(
                            Double.parseDouble(weightField.getText()),
                            Double.parseDouble(heightField.getText())
                    )));
                }
                else{
                    result.setText("Error!");
                }


            }
        });



        HBox root = new HBox(12, weightField, heightField, calcButton, result);
        root.setPadding(new Insets(10));
        root.setAlignment(Pos.CENTER);

        var scene = new Scene(root);

        stage.setTitle("BMI Calculator");
        stage.setScene(scene);
        stage.show();

    }

    public static void main(String[] args) {
        launch();
    }

}