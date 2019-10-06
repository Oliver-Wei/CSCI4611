# My solution to Assignment 2


Assigment 2:

1. Ball:

    a. Collision with walls
    
    For the collision of ball， I use different wall index to indicate different walls, which would help me to
    detect the collision between with different walls.

    The indecices that I used are:

         left wall: 0;
         right_wall: 1;
         ceiling: 2;
         ground: 3;
         front: 4;
         back: 5

    Then the position would be determined:
            if (wall_index == 0) {  // collision with left wall
                Vector3 norm = Vector3(1, 0, 0);
                Vector3 vel_after_colission = velocity_ - 2 * ((velocity_.Dot(norm)) * norm);
                velocity_ = vel_after_colission;
                
    b. Collision with cars:
    
    When the ball hit car, the collision normal would be calculated and then applied to the ball, the ball would firstly be       moved to a place that would not be influnced by the collision, then the velocity would add. 
    
    c. Goal
    
    When the ball goal, the reset function woudl be called to reset the ball to the initial position. 
    

2. Car
    
    a. Movement
       
    The move of the car is calculated by the three different functions: 
        void CarSoccer::OnSpecialKeyDown(int key, int scancode, int modifiers) {
        if (key == GLFW_KEY_SPACE) {
            // Here's where you could call some form of launch_ball();
            srand((unsigned)time(NULL));
            ball_.Reset();
            car_.Reset();
        }
        if (key == GLFW_KEY_LEFT) {
            car_.turning_left();

        }
        if (key == GLFW_KEY_RIGHT) {
            car_.turning_right();

        }
        if (key == GLFW_KEY_UP) {
            car_.move_forward();
        }
        if (key == GLFW_KEY_DOWN) {
            car_.move_back();

        }
        if (key == GLFW_KEY_R) {
            car_.press_r();
        }
    }

    void CarSoccer::OnSpecialKeyUp(int key, int scancode, int modifiers) {

        if (key == GLFW_KEY_UP) {
            //car_.move_forward();
            //std::cout << "UP" << std::endl;
        }
        if (key == GLFW_KEY_DOWN) {
            //car_.move_back();
            //std::cout << "DOWN" << std::endl;
        }
        if (key == GLFW_KEY_LEFT) {
            car_.reset_direction();
            //std::cout << "LEFT" << std::endl;
        }
        if (key == GLFW_KEY_RIGHT) {
            car_.reset_direction();
            //std::cout << "RIGHT" << std::endl;
        }


    }

    void CarSoccer::OnSpecialKeyRepeat (int key, int scancode, int modifiers) {

        if (key == GLFW_KEY_LEFT) {
            car_.turning_left();
        }
        if (key == GLFW_KEY_RIGHT) {
            car_.turning_right();
        }
        if (key == GLFW_KEY_UP) {
            car_.move_forward();
        }
        if (key == GLFW_KEY_DOWN) {
            car_.move_back();

        }
    }
    
    when the button is pressed, released and repeat, different action would take to move the car. 
    
    b. rotation. 
    
    I tried to rotation the car by a angular variable, but it could not catch the changing of velocity direction of the car. 
    
                
    
