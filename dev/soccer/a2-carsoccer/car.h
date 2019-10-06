/** CSci-4611 Assignment 2:  Car Soccer
 */

#ifndef CAR_H_
#define CAR_H_

#include <mingfx.h>

/// Small data structure for a car
class Car {
public:
    
    /// The constructor sets the static properties of the car, like its size,
    /// and then calls Reset() to reset the position, velocity, and any other
    /// dynamic variables that change during game play.
    Car() : size_(3,2,4), collision_radius_(2.5) {
        Reset();
    }

    /// Nothing special needed in the constructor
    virtual ~Car() {}

    /// Resets all the dynamic variables, so if you call this after a goal, the
    /// car will go back to its starting position.
    void Reset() {
        position_ = Point3(0, size_[1]/2, 45);
        Vector3 velocity_direction_ = Vector3(0, 0, -1);
        float velocity_magnitude_ = 0;
        Vector3 velocity_ = Vector3(0,0,0);
        Vector3 acceleration_ = Vector3(0,0,0);
    }
    
    float collision_radius() { return collision_radius_; }
    
    Vector3 size() { return size_; }
    
    Point3 position() { return position_; }
    Vector3 velocity() { return velocity_; }
    float magnitude() { return velocity_magnitude_; }
    float angular() { return angular_; }
    void set_position(const Point3 &p) { position_ = p; }
    void set_acceleration(const Vector3 &p) { acceleration_ = p;}
    
    void move(double timeStep) {
        
        position_ = position_ + velocity_ * timeStep ;
        
    }
    
    void accelerate(Vector3 acc) {
        
        acceleration_ = acceleration_ + acc;
        
    }
    
    void move_forward() {
    
        velocity_magnitude_ = velocity_magnitude_ + 1;
        velocity_ = velocity_magnitude_ * velocity_direction_;
        velocity_direction_ = velocity_.ToUnit();
        
    }
    
    void move_back() {
        
        velocity_magnitude_ = velocity_magnitude_ - 1;
        velocity_ = velocity_magnitude_ * velocity_direction_;
        velocity_direction_ = velocity_.ToUnit();

    }
    
    
    void initial_magnitude() {
     
        velocity_magnitude_ = 0;
    }
    
    void Collision_with_wall(int wall_index) {
        
        
        if (wall_index == 0) { //collisition with left wall
            velocity_magnitude_ = 0;
            position_ = Point3(-40+collision_radius(), position_.y(), position_.z());
        }
        if (wall_index == 1) { //collision with right wall
            velocity_magnitude_ = 0;
            position_ = Point3(40-collision_radius(), position_.y(), position_.z());
        }
        if (wall_index == 2) { //collision with front wall
            velocity_magnitude_ = 0;
            position_ = Point3(position_.x(), position_.y(), -50+collision_radius());
        }
        if (wall_index == 3) { //collision with back wall
            velocity_magnitude_ = 0;
            position_ = Point3(position_.x(), position_.y(), 50-collision_radius());
        }
    }
    
    void turning_left() {
        
        Vector3 turn_right_direction = (velocity_direction_.Cross(Vector3(0,-1,0))).ToUnit();
        velocity_direction_ = velocity().ToUnit();
        velocity_direction_ = (velocity_direction_ + turn_right_direction).ToUnit();
        velocity_ = velocity_direction_ * velocity_magnitude_;
        Turning_left_ = true;
        rotate_left();
    }
    
    bool left() {
        return Turning_left_;
    }
    
    void turning_right() {
    
        Vector3 turn_right_direction = (velocity_direction_.Cross(Vector3(0,1,0))).ToUnit();
        velocity_direction_ = velocity().ToUnit();
        velocity_direction_ = (velocity_direction_ + turn_right_direction).ToUnit();
        velocity_ = velocity_direction_ * velocity_magnitude_;
        Turning_right_ = true;
        rotate_right();
        
    }
    
    bool right() {
        return Turning_right_;
    }
    
    bool reset_direction() {
        
        Turning_left_ = false;
        Turning_right_ = false;
        
    }
    void press_r() {
        rotation_ = true;
    }
    
    void rotate_left() {
        
        angular_ = angular_ + 30;
        
    }
    
    void rotate_right() {
        
        angular_ = angular_ - 30;
        
    }
    
    
    
    
    
private:
    // You will probably need to store some additional data here, e.g., speed.
    
    Vector3 size_;
    float collision_radius_;
    Point3 position_;
    Vector3 velocity_direction_ = Vector3(0, 0, -1);
    float velocity_magnitude_ = 0;
    Vector3 velocity_ = Vector3(0,0,0);
    Vector3 acceleration_ = Vector3(0,0,0);
    bool Turning_left_ = false;
    bool Turning_right_ = false;
    float angular_ = 0;
    
    
    bool rotation_ = false;
};

#endif
