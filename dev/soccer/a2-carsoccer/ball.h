/** CSci-4611 Assignment 2:  Car Soccer
 */

#ifndef BALL_H_
#define BALL_H_

#include <mingfx.h>
#include   <stdlib.h>

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

        set_position(Point3(0,5,0));
        Vector3 velocity_ = Vector3(rand() % 31 - 15, rand() % 5 + 5, rand() % 31 - 15);
        Vector3 acceleration_ = Vector3(0,0,0);
    }

    float radius() { return radius_; }
    
    Point3 position() { return position_; }
    
    void set_position(const Point3 &p) { position_ = p; }
    
    void move(double timeStep) {
        
        position_ = position_ + velocity_ * timeStep;
        
    }
    
    Vector3 accelerate(double timestep) {
        
        velocity_ = velocity_ + acceleration_ * timestep;
        return velocity_;

        
    }
    
    void Collision_with_wall(int wall_index) {
        
        // left wall: 0;
        // right_wall: 1;
        // ceiling: 2;
        // ground: 3;
        // front: 4;
        // back: 5
        
        if (wall_index == 0) {  // collision with left wall
            Vector3 norm = Vector3(1, 0, 0);
            Vector3 vel_after_colission = velocity_ - 2 * ((velocity_.Dot(norm)) * norm);
            velocity_ = vel_after_colission;
            //printf("left collision----------------------\n");
           
        }
        else if (wall_index == 1) { //collision with right wall
            Vector3 norm = Vector3(-1, 0, 0);
            Vector3 vel_after_colission = velocity_ - 2 * ((velocity_.Dot(norm)) * norm);
            velocity_ = vel_after_colission;
            //printf("right collision---------------------\n");
          
        }
        else if (wall_index == 2) { //collision with ceiling
            
            Vector3 norm = Vector3(0, -1, 0);
            Vector3 vel_after_colission = velocity_ - 2 * ((velocity_.Dot(norm)) * norm);
            velocity_ = vel_after_colission;
            //printf("ceilling collision------------------\n");
            
            
        }
        else if (wall_index == 3) { //collision with ground
            
            Vector3 norm = Vector3(0, 1, 0);
            Vector3 vel_after_colission = velocity_ - 2 * ((velocity_.Dot(norm)) * norm);
            velocity_ = vel_after_colission;
            //printf("ground collision--------------------\n");
            
            
        }
        
        else if (wall_index == 4) { // collision with front
            
            Vector3 norm = Vector3(0, 0, 1);
            Vector3 vel_after_colission = velocity_ - 2 * ((velocity_.Dot(norm)) * norm);
            velocity_ = vel_after_colission;
            //printf("front collision---------------------\n");
            
        }
        
        else if (wall_index == 5) { //collision with back
            
            Vector3 norm = Vector3(0, 0, -1);
            Vector3 vel_after_colission = velocity_ - 2 * ((velocity_.Dot(norm)) * norm);
            velocity_ = vel_after_colission;
            //printf("back collision----------------------\n");
            
            
        }
    }
    
    void ball_collision_with_car(Vector3 norm, Vector3 car_velocity, Point3 car_position, float sum_of_radius) {
        
        norm = norm.ToUnit();
        while ((car_position - position_).Length() < sum_of_radius) {
            position_ = position_ + norm * 0.1;
        }
        Vector3 vel_after_collision = velocity_ - 2 * ((velocity_.Dot(norm)) * norm) + car_velocity;
        velocity_ = vel_after_collision;
        
    }
    
    void gravity_acc(double timestep) {
        
        if (velocity_.y() != 0) {
            Vector3 grav_norm = Vector3(0, -1, 0);
            Vector3 gravity_acc = grav_norm * 5;
            velocity_ = velocity_ + gravity_acc * timestep;
        }
        
    }
    
    void friction() {
        
        if (velocity_ != Vector3(0,0,0)) {
            velocity_ = velocity_ - Vector3(velocity_.x() * 0.0001, velocity_.y() * 0.0001, velocity_.z() * 0.0001);
        }
        
    }
    
    Vector3 velocity() {
        
        return velocity_;
    }
    
    void Goal() {
        Reset();
    }
    

    
    
private:
    // You will probably need to store some additional data here, e.g., velocity
    
    float radius_;
    Point3 position_ = Point3(0, 10, 0);
    Vector3 velocity_ = Vector3(rand() % 31 - 15, rand() % 5 + 5, rand() % 31 - 15);
    Vector3 acceleration_ = Vector3(0,0,0);
    
    
};

#endif
