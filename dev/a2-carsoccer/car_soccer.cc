/** CSci-4611 Assignment 2:  Car Soccer
 */

#include "car_soccer.h"
#include "config.h"


CarSoccer::CarSoccer() : GraphicsApp(1024,768, "Car Soccer") {
    // Define a search path for finding data files (images and shaders)
    searchPath_.push_back(".");
    searchPath_.push_back("./data");
    searchPath_.push_back(DATA_DIR_INSTALL);
    searchPath_.push_back(DATA_DIR_BUILD);
}

CarSoccer::~CarSoccer() {
}


Vector2 CarSoccer::joystick_direction() {
    Vector2 dir;
    if (IsKeyDown(GLFW_KEY_LEFT))
        dir[0]--;
    if (IsKeyDown(GLFW_KEY_RIGHT))
        dir[0]++;
    if (IsKeyDown(GLFW_KEY_UP))
        dir[1]++;
    if (IsKeyDown(GLFW_KEY_DOWN))
        dir[1]--;
    return dir;
}


void CarSoccer::OnSpecialKeyDown(int key, int scancode, int modifiers) {
    if (key == GLFW_KEY_SPACE) {
        // Here's where you could call some form of launch_ball();
        ball_.Reset();
        car_.Reset();
    }
    if (key == GLFW_KEY_LEFT) {
        car_.turn_left();
        
    }
    if (key == GLFW_KEY_RIGHT) {
        car_.turn_right();
        
    }
    if (key == GLFW_KEY_UP) {
        car_.speed_up();
    }
    if (key == GLFW_KEY_DOWN) {
        car_.speed_down();
        
    }
}

void CarSoccer::OnSpecialKeyRepeat (int key, int scancode, int modifiers) {
    
    if (key == GLFW_KEY_LEFT) {
        car_.turn_left();
    }
    if (key == GLFW_KEY_RIGHT) {
        car_.turn_right();
    }
    if (key == GLFW_KEY_UP) {
        car_.speed_up();
    }
    if (key == GLFW_KEY_DOWN) {
        car_.speed_down();
        
    }
}


void CarSoccer::UpdateSimulation(double timeStep) {
    // Here's where you shound do your "simulation", updating the positions of the
    // car and ball as needed and checking for collisions.  Filling this routine
    // in is the main part of the assignment.
    
    ball_.gravity_acc(timeStep);
    ball_.move(timeStep);
    
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
    
    if (ball_.position().y() + ball_.radius() >= 35) {
        
        //ceiling
        
        ball_.Collision_with_wall(2);
        
    }
    else if (ball_.position().y() - ball_.radius() <= 0) {
        
        //ground
        
        ball_.Collision_with_wall(3);
    }
    
    else if (ball_.position().x() - ball_.radius() <= -40) {
        
        // left
        
        ball_.Collision_with_wall(0);
        
    }
    
    else if (ball_.position().x() + ball_.radius() >= 40) {
        
        // right
        
        ball_.Collision_with_wall(1);
    }
    
    else if (ball_.position().z() + ball_.radius() >= 50) {
        
        // back
        
        ball_.Collision_with_wall(5);
    }
    
    else if (ball_.position().z() - ball_.radius() <= -50) {
        
        // front
        
        ball_.Collision_with_wall(4);
    }
    
    if ((ball_.position()-car_.position()).Length() <= ball_.radius() + car_.collision_radius()) {
        
        float sum_of_radius = ball_.radius() + car_.collision_radius();
        Vector3 collision_normal;
        collision_normal = (ball_.position() - car_.position()).ToUnit();
        
        ball_.ball_collision_with_car(collision_normal, car_.velocity(), car_.position(), sum_of_radius);
    }
    
    
    car_.move(timeStep);
    if (car_.position().x() - car_.collision_radius() <= -40){
        
        //left wall
        car_.Collision_with_wall(0);
        
    }
    else if (car_.position().x() + car_.collision_radius() >= 40){
        
        //right wall
        car_.Collision_with_wall(1);
        
    }
    if (car_.position().z() - car_.collision_radius() <= -50){
        
        //front wall
        car_.Collision_with_wall(2);
        
    }
    else if (car_.position().z() + car_.collision_radius() >= 50) {
        
        //back wall
        car_.Collision_with_wall(3);
    }
    
}


void CarSoccer::InitOpenGL() {
    // Set up the camera in a good position to see the entire field
    projMatrix_ = Matrix4::Perspective(60, aspect_ratio(), 1, 1000);
    modelMatrix_ = Matrix4::LookAt(Point3(0,60,70), Point3(0,0,10), Vector3(0,1,0));
 
    // Set a background color for the screen
    glClearColor(0.8,0.8,0.8, 1);
    
    // Load some image files we'll use
    fieldTex_.InitFromFile(Platform::FindFile("pitch.png", searchPath_));
    crowdTex_.InitFromFile(Platform::FindFile("crowd.png", searchPath_));
}


void CarSoccer::DrawUsingOpenGL() {
    // Draw the crowd as a fullscreen background image
    quickShapes_.DrawFullscreenTexture(Color(1,1,1), crowdTex_);
    
    // Draw the field with the field texture on it.
    Color col(16.0/255.0, 46.0/255.0, 9.0/255.0);
    Matrix4 M = Matrix4::Translation(Vector3(0,-0.201,0)) * Matrix4::Scale(Vector3(50, 1, 60));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, col);
    M = Matrix4::Translation(Vector3(0,-0.2,0)) * Matrix4::Scale(Vector3(40, 1, 50));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, Color(1,1,1), fieldTex_);
    
    // Draw the car
    Color carcol(0.8, 0.2, 0.2);
    Matrix4 Mcar =
        Matrix4::Translation(car_.position() - Point3(0,0,0)) *
        Matrix4::Rotation(Point3(0,0,0), Vector3(0,1,0), car_.angle()) *
        Matrix4::Scale(car_.size()) *
        Matrix4::Scale(Vector3(0.5,0.5,0.5)) ;
    quickShapes_.DrawCube(modelMatrix_ * Mcar, viewMatrix_, projMatrix_, carcol);
    quickShapes_.DrawArrow(modelMatrix_, viewMatrix_, projMatrix_, carcol,car_.position(),5*car_.velocity_direction(),0.1);
    
    
    // Draw the ball
    Color ballcol(1,1,1);
    Matrix4 Mball =
        Matrix4::Translation(ball_.position() - Point3(0,0,0)) *
        Matrix4::Scale(Vector3(ball_.radius(), ball_.radius(), ball_.radius()));
    quickShapes_.DrawSphere(modelMatrix_ * Mball, viewMatrix_, projMatrix_, ballcol);
//    quickShapes_.DrawArrow(modelMatrix_, viewMatrix_, projMatrix_, ballcol,ball_.position(),ball_.velocity(),0.1);
    
    
    // Draw the ball's shadow -- this is a bit of a hack, scaling Y by zero
    // flattens the sphere into a pancake, which we then draw just a bit
    // above the ground plane.
    Color shadowcol(0.2,0.4,0.15);
    Matrix4 Mshadow =
        Matrix4::Translation(Vector3(ball_.position()[0], -0.1, ball_.position()[2])) *
        Matrix4::Scale(Vector3(ball_.radius(), 0, ball_.radius())) *
        Matrix4::RotationX(90);
    quickShapes_.DrawSphere(modelMatrix_ * Mshadow, viewMatrix_, projMatrix_, shadowcol);
    
    
//    // You should add drawing the goals and the boundary of the playing area
//    // using quickShapes_.DrawLines()
    
    Color line(1, 1, 1);
    std::vector<Point3> Right_part;
    Right_part.push_back(Point3(40, 0, -50));
    Right_part.push_back(Point3(40, 35, -50));
    Right_part.push_back(Point3(40, 35, 50));
    Right_part.push_back(Point3(40, 0, 50));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, line, Right_part, QuickShapes::LinesType::LINE_LOOP, 0.1);
    
    std::vector<Point3> Down_part;
    Down_part.push_back(Point3(-40, 0, 50));
    Down_part.push_back(Point3(-40, 35, 50));
    Down_part.push_back(Point3(40, 35, 50));
    Down_part.push_back(Point3(40, 0, 50));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, line, Down_part, QuickShapes::LinesType::LINE_LOOP, 0.1);

    std::vector<Point3> Left_part;
    Left_part.push_back(Point3(-40, 0, 50));
    Left_part.push_back(Point3(-40, 35, 50));
    Left_part.push_back(Point3(-40, 35, -50));
    Left_part.push_back(Point3(-40, 0, -50));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, line, Left_part, QuickShapes::LinesType::LINE_LOOP, 0.1);

    std::vector<Point3> Upper_part;
    Upper_part.push_back(Point3(-40, 35, -50));
    Upper_part.push_back(Point3(-40, 0, -50));
    Upper_part.push_back(Point3(40, 0, -50));
    Upper_part.push_back(Point3(40, 35, -50));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, line, Upper_part, QuickShapes::LinesType::LINE_LOOP, 0.1);

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
    
    std::vector<Point3> Down_Goal;
    Color down_goal(0.7,0.5,0);
    int down_goal_x;
    int down_goal_y;
    for (down_goal_x = -10;down_goal_x <= 10;down_goal_x = down_goal_x + 1) {
        quickShapes_.DrawLineSegment(modelMatrix_, viewMatrix_, projMatrix_, down_goal, Point3(down_goal_x, 10, 50), Point3(down_goal_x, 0, 50), 0.1);
    }
    for (down_goal_y = 0;down_goal_y <= 10;down_goal_y = down_goal_y + 1) {
        quickShapes_.DrawLineSegment(modelMatrix_, viewMatrix_, projMatrix_, down_goal, Point3(-10, down_goal_y, 50), Point3(10, down_goal_y, 50), 0.1);
    }

}
