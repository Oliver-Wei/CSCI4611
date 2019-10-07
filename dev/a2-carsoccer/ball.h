/** CSci-4611 Assignment 2:  Car Soccer
 */

#ifndef BALL_H_
#define BALL_H_

#include <mingfx.h>
#include <time.h>
#include <stdlib.h>

/// Small data structure for a ball
class Ball {
public:
    
    /// The constructor sets the radius and calls Reset() to start the ball at
    /// the center of the field
    Ball() : radius_(2.6) {
        Reset();
    }
    
    /// Nothing special needed in the constructor
    virtual ~Ball() {}

    
    void Reset() {
        position_ = Point3(0, radius_, 0);
        set_position(Point3(0,5,0));
        srand(time(NULL));
        Vector3 velocity_ = Vector3(rand() % 31 - 15, rand() % 5 + 5, rand() % 31 - 15);
        Vector3 acceleration_ = Vector3(0,0,0);
    }

    float radius() { return radius_; }
    
    Point3 position() { return position_; }
    
    void set_position(const Point3 &p) { position_ = p; }
    
    void move(double timeStep) {
        position_ = position_ + velocity_ * timeStep;
    }
    
    void Collision_with_wall(int wall_index) {
        
        // left wall: 0;
        // right_wall: 1;
        // ceiling: 2;
        // ground: 3;
        // front: 4;
        // back: 5
        Vector3 norm;
        
        if (wall_index == 0) {  // collision with left wall
            norm = Vector3(1, 0, 0);
            position_ = Point3(-40 + radius(),position_.y(),position_.z());
            velocity_ = velocity_ - 2 * ((velocity_.Dot(norm)) * norm);
            
        }
        else if (wall_index == 1) { //collision with right wall
            norm = Vector3(-1, 0, 0);
            position_ = Point3(40 - radius(),position_.y(),position_.z());
            velocity_ = velocity_ - 2 * ((velocity_.Dot(norm)) * norm);
            
        }
        else if (wall_index == 2) { //collision with ceiling
            
            norm = Vector3(0, -1, 0);
            position_ = Point3(position_.x(),35 - radius(),position_.z());
            velocity_ = velocity_ - 2 * ((velocity_.Dot(norm)) * norm);
            
            
        }
        else if (wall_index == 3) { //collision with ground
            
            norm = Vector3(0, 1, 0);
            position_ = Point3(position_.x(),radius(),position_.z());
            velocity_ = velocity_ - 2 * ((velocity_.Dot(norm)) * norm);
            
            
        }
        
        else if (wall_index == 4) { // collision with front
            
            norm = Vector3(0, 0, 1);
            position_ = Point3(position_.x(),position_.y(),-50 + radius());
            velocity_ = velocity_ - 2 * ((velocity_.Dot(norm)) * norm);
            
        }
        
        else if (wall_index == 5) { //collision with back
            
            norm = Vector3(0, 0, -1);
            position_ = Point3(position_.x(),position_.y(), 50 - radius());
            velocity_ = velocity_ - 2 * ((velocity_.Dot(norm)) * norm);
            
        }
        friction();
    }
    
    void ball_collision_with_car(Vector3 norm, Vector3 car_velocity, Point3 car_position, float sum_of_radius) {
        
        while ((car_position - position_).Length() < sum_of_radius) {
            position_ = position_ + norm * 0.1;
        }
        Vector3 vel_rev = velocity_ - car_velocity;
        velocity_ = car_velocity + vel_rev - 2 * ((vel_rev.Dot(norm)) * norm);
        friction();
        
    }
    
    void gravity_acc(double timestep) {
        
        if (velocity_.y() != 0) {
            Vector3 grav_norm = Vector3(0, -1, 0);
            Vector3 gravity_acc = grav_norm * 5;
            velocity_ = velocity_ + gravity_acc * timestep;
        }
        
    }
    
    void friction() {
        
        velocity_ = velocity_ * 0.8;
        
    }
    
    Vector3 velocity() {
        
        return velocity_;
    }
    
    void Goal() {
        Reset();
    }
    
    
    
    
private:
    // You will probably need to store some additional data here, e.g., velocity
    
    Point3 position_ = Point3(0, 10, 0);
    float radius_;
    Vector3 velocity_ = Vector3(rand() % 31 - 15, rand() % 5 + 5, rand() % 31 - 15);
    Vector3 acceleration_ = Vector3(0,0,0);
};

#endif
