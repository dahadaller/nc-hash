package com.nchash.model;

import javafx.beans.property.DoubleProperty;
import javafx.beans.property.SimpleDoubleProperty;
import javafx.geometry.Point2D;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

public class Particle {
    private DoubleProperty x = new SimpleDoubleProperty();
    private DoubleProperty y = new SimpleDoubleProperty();
    private Color color;
    private boolean active = false;

    // Velocity for each particle.
    private Point2D velocity = Point2D.ZERO;

    // Value for the alpha
    private double life = 1.0;

    public Particle(int x, int y, Color color){
        this.x.set(x);
        this.y.set(y);
        this.color = color;
    }

    public double getX() {
        return x.get();
    }

    public DoubleProperty xProperty() {
        return x;
    }

    public void setX(double x) {
        this.x.set(x);
    }

    public double getY() {
        return y.get();
    }

    public DoubleProperty yProperty() {
        return y;
    }

    public void setY(double y) {
        this.y.set(y);
    }

    public Color getColor() {
        return color;
    }

    public void setColor(Color color) {
        this.color = color;
    }

    /**
     * Lets us draw the particle.
     * @param g
     */
    public void draw(GraphicsContext g){
        g.setFill(color);
        g.setGlobalAlpha(life);
        g.fillOval(getX(), getY(), 1, 1);
    }

    /**
     * Lets us know if the particle is dead. If its dead we can remove it
     * from the screen, thus increasing performance.
     * @return
     */
    public boolean isDead(){
        return life == 0;
    }

    /**
     * Allows us to activate the particle.
     */
    public void activate(Point2D velocity){
        active = true;
        this.velocity = velocity;
    }

    public boolean isActive(){
        return active;
    }

    public void update(){
        if (!active){
            return;
        }
        /*
        Reduces the value by every 60th of a second times 0.75
        So that is slightly smaller than that.
         */
        life -= 0.017 * 0.75;

        if (life < 0){
            life = 0;
        }

        this.x.set(getX() + velocity.getX());
        this.y.set(getY() + velocity.getY());
    }
}
