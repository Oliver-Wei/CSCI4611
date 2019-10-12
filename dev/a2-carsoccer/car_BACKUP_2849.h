/** CSci-4611 Assignment 2:  Car Soccer
 */

#ifndef CAR_H_
#define CAR_H_

#include <mingfx.h>
#include <math.h>

#define PI 3.14159265

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
        velocity_direction_ = Vector3(0, 0, -1);
        velocity_magnitude_ = 0;
        velocity_angle_ = 0;
        velocity_ = Vector3(0,0,0);
    }
    
    float collision_radius() { return collision_radius_; }
    
    Vector3 size() { return size_; }
    
    Point3 position() { return position_; }
    Vector3 velocity() { return velocity_; }
    Vector3 velocity_direction() {return velocity_direction_;}
    float angle() {return velocity_angle_;}
    float magnitude() { return velocity_magnitude_; }
    
    void set_position(const Point3 &p) { position_ = p; }
    
<<<<<<< HEAD
    void move(double timeStep) {
        
        position_ = position_ + velocity_ * timeStep ;
        
    }
    
    void turn_left() {
        //turn it at a rate proportional to its speed
        Vector3 turn_left_direction = (velocity_direction_.Cross(Vector3(0,-1,0)));
        velocity_direction_ = (velocity_direction_ + turn_left_direction * velocity_magnitude_/10).ToUnit();
        velocity_ = velocity_direction_ * velocity_magnitude_;
        velocity_angle_ = velocity_angle_ + PI/2 - acos(velocity_direction_.Dot(turn_left_direction));
    }
    
    
    void turn_right() {
        //turn it at a rate proportional to its speed
        Vector3 turn_right_direction = (velocity_direction_.Cross(Vector3(0,1,0)));
        velocity_direction_ = (velocity_direction_ + turn_right_direction * velocity_magnitude_/10).ToUnit();
        velocity_ = velocity_direction_ * velocity_magnitude_;
        velocity_angle_ = velocity_angle_ - (PI/2 - acos(velocity_direction_.Dot(turn_right_direction)));
    }
    
    void speed_up() {
        if (velocity_magnitude_ < 20){
            velocity_magnitude_ = velocity_magnitude_ + 1;
        }
        velocity_ = velocity_magnitude_ * velocity_direction_;
    }
    
    void speed_down() {
        if (velocity_magnitude_ > -20) {
            velocity_magnitude_ = velocity_magnitude_ - 1;
        }
        velocity_ = velocity_magnitude_ * velocity_direction_;
    }
    
    void Collision_with_wall(int wall_index) {
        
        if (wall_index == 0) { //collision with left wall
            Vector3 norm = Vector3(1, 0, 0);
            position_ = Point3(-40 + collision_radius(), position_.y(), position_.z());
            velocity_ = velocity_ - 2 * ((velocity_.Dot(norm)) * norm);
            if (velocity_magnitude_ > 0){
                velocity_direction_ = velocity_.ToUnit();
            }
            else {
                velocity_direction_ = -velocity_.ToUnit();
            }
            if (velocity_direction_.z() <= 0) {
                velocity_angle_ = velocity_angle_ - (PI - 2*acos(velocity_direction_.Dot(norm)));
            }
            else {
                velocity_angle_ = velocity_angle_ + (PI - 2*acos(velocity_direction_.Dot(norm)));
            }
        }
        if (wall_index == 1) { //collision with right wall
            Vector3 norm = Vector3(-1, 0, 0);
            position_ = Point3(40 - collision_radius(), position_.y(), position_.z());
            velocity_ = velocity_ - 2 * ((velocity_.Dot(norm)) * norm);
            if (velocity_magnitude_ > 0){
                velocity_direction_ = velocity_.ToUnit();
            }
            else {
                velocity_direction_ = -velocity_.ToUnit();
            }
            if (velocity_direction_.z() <= 0) {
                velocity_angle_ = velocity_angle_ + (PI - 2*acos(velocity_direction_.Dot(norm)));
            }
            else {
                velocity_angle_ = velocity_angle_ - (PI - 2*acos(velocity_direction_.Dot(norm)));
            }
            
        }
        if (wall_index == 2) { //collision with front wall
            Vector3 norm = Vector3(0, 0, 1);
            position_ = Point3(position_.x(), position_.y(), -50 + collision_radius());
            velocity_ = velocity_ - 2 * ((velocity_.Dot(norm)) * norm);
            if (velocity_magnitude_ > 0){
                velocity_direction_ = velocity_.ToUnit();
            }
            else {
                velocity_direction_ = -velocity_.ToUnit();
            }
            if (velocity_direction_.x() <= 0) {
                velocity_angle_ = velocity_angle_ + (PI - 2*acos(velocity_direction_.Dot(norm)));
            }
            else {
                velocity_angle_ = velocity_angle_ - (PI - 2*acos(velocity_direction_.Dot(norm)));
            }

            
        }
        if (wall_index == 3) { //collision with back wall
            Vector3 norm = Vector3(0, 0, -1);
            position_ = Point3(position_.x(), position_.y(), 50 - collision_radius());
            velocity_ = velocity_ - 2 * ((velocity_.Dot(norm)) * norm);
            if (velocity_magnitude_ > 0){
                velocity_direction_ = velocity_.ToUnit();
            }
            else {
                velocity_direction_ = -velocity_.ToUnit();
            }
            if (velocity_direction_.x() <= 0) {
                velocity_angle_ = velocity_angle_ - (PI - 2*acos(velocity_direction_.Dot(norm)));
            }
            else {
                velocity_angle_ = velocity_angle_ + (PI - 2*acos(velocity_direction_.Dot(norm)));
            }

        }
        
        friction();
    }
    
    void friction() {
        
        velocity_ = velocity_ * 0.8;
        
    }
    
    
private:
    // You will probably need to store some additional data here, e.g., speed.
    
    Vector3 size_;
    float collision_radius_;
    Point3 position_;
    
    Vector3 velocity_direction_ = Vector3(0, 0, -1);
    float velocity_magnitude_ = 0;
    float velocity_angle_ = 0;
    Vector3 velocity_ = Vector3(0,0,0);
    
    
};

#endif
