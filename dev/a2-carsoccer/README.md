# Assignment 2 


Assigment 2 
Yingxin Wei

1. Pitch and goal

I use the QuickShapes::DrawLines() routine to draw lines around the boundary of the 3D soccer pitch so that we can see the box that we will be playing inside. For a box, there should be 6 sides, but I can achieve the same effect by only drawing four rectangles. The following is how I draw one of the four rectangles.

    Color line(1, 1, 1);
    std::vector<Point3> Right_part;
    Right_part.push_back(Point3(40, 0, -50));
    Right_part.push_back(Point3(40, 35, -50));
    Right_part.push_back(Point3(40, 35, 50));
    Right_part.push_back(Point3(40, 0, 50));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, line, Right_part, QuickShapes::LinesType::LINE_LOOP, 0.1);

I use the same method to draw a grid of lines for each goal.

    std::vector<Point3> Upper_Goal;
    Color upper_goal(0,0.5,0.5);
    int upper_goal_x;
    int upper_goal_y;
    for (upper_goal_x = -10;upper_goal_x <= 10;upper_goal_x = upper_goal_x + 1) {
        quickShapes_.DrawLineSegment(modelMatrix_, viewMatrix_, projMatrix_, upper_goal, Point3(upper_goal_x, 10, -50), Point3(upper_goal_x, 0, -50), 0.1);
    }
    for (upper_goal_y = 0;upper_goal_y <= 10;upper_goal_y = upper_goal_y + 1) {
        quickShapes_.DrawLineSegment(modelMatrix_, viewMatrix_, projMatrix_, upper_goal, Point3(-10, upper_goal_y, -50), Point3(10, upper_goal_y, -50), 0.1);
    }

2. Ball

In my program, the member `velocity_` records the current velocity of the ball, the member `position_` denotes the current position of the ball. The relationship between these two members is as follows.

    void move(double timeStep) {
        position_ = position_ + velocity_ * timeStep ;
    }
    
During normal movement, the member `velocity_`  is only affected by the gravity.

    void gravity_acc(double timestep) {
        if (velocity_.y() != 0) {
            Vector3 grav_norm = Vector3(0, -1, 0);
            Vector3 gravity_acc = grav_norm * 5;
            velocity_ = velocity_ + gravity_acc * timestep;
        }
    }

The initial position and velocity of the ball is defined as follows. Its initial position is at the center of the ground. Its initial velocity is random but not totally random. The ball should move towards my side (camera side).

    void Reset() {
        position_ = Point3(0, radius_, 0);
        set_position(Point3(0,5,0));
        Vector3 velocity_ = Vector3(rand() % 31 - 15, rand() % 5 + 5, rand() % 31);
        Vector3 acceleration_ = Vector3(0,0,0);
    }
    
The `velocity_` of the ball is decreased by a constant factor when it bounces.

    void friction() {
        velocity_ = velocity_ * 0.8;
    }

a. Collision with walls, ceiling and ground
    
The walls, ceiling and ground can be regarded as the same thing, "wall". Therefore, This pitch has six "walls" in total. Each of them has its own index and normal vector. When the ball hits any of the "walls", the ball’s `velocity_` will be reflected about the normal vector. And the `position_` of the ball should be corrected if at the next frame the ball not only hits the wall but is partially inside it. I correct it by setting `position_` to a value that places the ball just outside the wall

The indices that I use are:

     left wall: 0;
     right_wall: 1;
     ceiling: 2;
     ground: 3;
     front: 4;
     back: 5

An example of how the ball collides with "walls". In this example, the `position_` is corrected by `position_ = Point3(-40 + collision_radius(), position_.y(), position_.z());`. 
    
    if (wall_index == 0) {  // collision with left wall
        norm = Vector3(1, 0, 0);
        position_ = Point3(-40 + radius(),position_.y(),position_.z());
        velocity_ = velocity_ - 2 * ((velocity_.Dot(norm)) * norm);
    }
    friction();

                
b. Collision with the car:
    
When the ball collides with the car, the collision normal should be calculated and the collision process should be simulated as illustrated in the handout. The codes below can also incorporate the velocity of the car into the way the ball responds when hit by using `vel_rel` (the relative velocity of the ball). I also make a small modification that the ball is placed to the nearest position to the place where two objects just contacts, and then collision happens.

    if ((ball_.position()-car_.position()).Length() <= ball_.radius() + car_.collision_radius()) {
        float sum_of_radius = ball_.radius() + car_.collision_radius();
        Vector3 collision_normal;
        collision_normal = (ball_.position() - car_.position()).ToUnit();
        ball_.ball_collision_with_car(collision_normal, car_.velocity(), car_.position(), sum_of_radius);
    }
    
    void ball_collision_with_car(Vector3 norm, Vector3 car_velocity, Point3 car_position, float sum_of_radius) {
        while ((car_position - position_).Length() < sum_of_radius) {
            position_ = position_ + norm * 0.1;
        }
        Vector3 vel_rev = velocity_ - car_velocity;
        velocity_ = car_velocity + vel_rev - 2 * ((vel_rev.Dot(norm)) * norm);
        friction();
    }


3. Car

Besides the position, size and collision radius of the car, in order to simulate the movement of the car, it has four main properties, `velocity_` (a vector), `velocity_direction_` (a unit vector showing the direction of the velocity) and `velocity_magnitude_` (a float number represents the speed of the velocity), and `velocity_angle_` (a float number helps identify the orientation of the car).

The function `float angle() {return velocity_angle_;}` gives us the orientation of the car and the following codes show how I draw the car according to its position and orientation.


    // Draw the car
    Color carcol(0.8, 0.2, 0.2);
    Matrix4 Mcar =
    Matrix4::Translation(car_.position() - Point3(0,0,0)) *
    Matrix4::Rotation(Point3(0,0,0), Vector3(0,1,0), car_.angle()) *
    Matrix4::Scale(car_.size()) *
    Matrix4::Scale(Vector3(0.5,0.5,0.5)) ;
    quickShapes_.DrawCube(modelMatrix_ * Mcar, viewMatrix_, projMatrix_, carcol);
    quickShapes_.DrawArrow(modelMatrix_, viewMatrix_, projMatrix_, carcol,car_.position(),car_.velocity(),0.1);

a. Movement

The movement of the car is controlled by the user. The `position_` and `velocity_` of the car is also related by the following equation.

    void move(double timeStep) {
        position_ = position_ + velocity_ * timeStep ;
    }

When the `Reset()` is called, the car should return to its inital state.

    void Reset() {
        position_ = Point3(0, size_[1]/2, 45);
        velocity_direction_ = Vector3(0, 0, -1);
        velocity_magnitude_ = 0;
        velocity_angle_ = 0;
        velocity_ = Vector3(0,0,0);
    }
    
The car model is mainly simulated as described in the handout. The car should always move forwards or backwards relative to the direction it’s facing, but never sideways. The up and down arrow keys should change its speed, while left and right should turn it at a rate proportional to its speed. My model may be slightly different. When the car moves forwards, an arrow shows the moving direction of it. When the car moves backwards, an arraw shows the opposite direction of the moving direction of it. When the left or right arrow key is pressed, the car always turns left or right like a real car no matter it moves forwards or backwards. The functions of how the car turns left and right and how it moves forwards and backwards are shown below. I also set a speed limit to the car.

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


    
b. Collision with the wall

When the car hits the walls, it follows the similar behavior as the ball. No matter how the car hits the wall, with its head or tail, the behaviors are the same. Here is an example of how the car hits the left wall.
    
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
    friction();

4. Goal

When the ball hits one of the goals, reset the car to the initial position and relaunch the ball from the center of the pitch.

    // Goal
    if (ball_.position().y() < 20 - ball_.radius()) {
        if (ball_.position().z() - ball_.radius() <= -50) {
            if (ball_.position().x() > -10 && ball_.position().x() < 10) {
                ball_.Reset();
                car_.Reset();
            }
        }
        else if (ball_.position().z() + ball_.radius() >= 50) {
            if (ball_.position().x() > -10 && ball_.position().x() < 10) {
            ball_.Goal();
            car_.Reset();
            }
        }
    }
    
    

    
                
    
