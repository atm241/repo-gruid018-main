/** CSci-4611 Assignment 2:  Car Soccer
 */

#include "car_soccer.h"
#include "config.h"
#include <iostream>


// Remember in C++, the .h file list all the functions and member variables that are part of the class!
// Look there first to understand what is part of the CarSoccer class, then look below to see how each
// function is implemented.


CarSoccer::CarSoccer() : GraphicsApp(1024,768, "Car Soccer") {
    // If you are having trouble driving the car with the keybaord, you can set this to true to use
    // the mouse instead.  The mouse controls are based on the postion of the mouse cursor on the window.
    // There is a "dead zone" in the middle of the window, and if you move the mouse up/down or left/right
    // outside of that zone, it is like pushing the up/down and/or left/right keys on the keyboard
    use_mouse_ = false;
        
    // Define a search path for finding data files (images and shaders)
    searchPath_.push_back(".");
    searchPath_.push_back("./data");
    searchPath_.push_back(DATA_DIR_INSTALL);
    searchPath_.push_back(DATA_DIR_BUILD);
}

CarSoccer::~CarSoccer() {
}


void CarSoccer::OnMouseMove(const Point2& pos, const Vector2& delta)
{
    mouse_pos_ = PixelsToNormalizedDeviceCoords(pos);
}

void CarSoccer::OnSpecialKeyDown(int key, int scancode, int modifiers) {
    if (key == GLFW_KEY_SPACE) {
        // Here's where you could call some form of launch_ball();
        ball_.Reset();
    }
}

/// This is a little utility function that is helpful.  It treats the arrow keys like a joystick or D-pad on a game controller
/// and returns the direction you are pressing as a 2D vector, taking into account the fact that you might be holding
/// down more than one key at a time.
Vector2 CarSoccer::joystick_direction() {
    Vector2 dir;

    if (use_mouse_) {
        // threshold defines the size of the "dead zone" in the middle of the screen
        // if the mouse's x,y position falls outside of this, then it is like pushing
        // the corresponding key on the keyboard
        const float threshold = 0.2f;
        dir[0] = 0;
        if (mouse_pos_[0] < -threshold) {
            dir[0] = -1;
        }
        else if (mouse_pos_[0] > threshold) {
            dir[0] = 1;
        }
        dir[1] = 0;
        if (mouse_pos_[1] < -threshold) {
            dir[1] = -1;
        }
        else if (mouse_pos_[1] > threshold) {
            dir[1] = 1;
        }
    }
    else {
        // the default user interface is to use the arrow keys on the keyboard.
        // like a D-pad on a game controller, you can hold more than one key down at a time if you want.
       
        //left right control steering dir[0] -1 = left, 1 - right
        if (IsKeyDown(GLFW_KEY_LEFT))
            dir[0]--; 
        if (IsKeyDown(GLFW_KEY_RIGHT))
            dir[0]++;

        //up down control steering dir[1] -1 = down, 1 = up
        if (IsKeyDown(GLFW_KEY_UP))
            dir[1]++;
        if (IsKeyDown(GLFW_KEY_DOWN))
            dir[1]--;
    }

    return dir;
}

// dt is for "Delta Time", the elapsed time in seconds since the last frame
void CarSoccer::UpdateSimulation(double dt) {
    Vector2 dpad_dir = joystick_direction();
    std::cout << "D-Pad Direction: " << dpad_dir << std::endl;
    
    /*
    purple_goal = (10, 0, 50);
    (-10, 0, 50)
    (-10, 10, 50)
    (10, 10, 50)

    //blue goal = (10, 0, -50);
    (-10, 0, -50)
    (-10, 10, -50)
    (10, 10, -50)
    */

    // Here's where you shound do your "simulation", updating the positions of the
    // car and ball based on the elapsed time and checking for collisions.  Filling
    // in this routine is the main part of the assignment.
    ball_.set_position(ball_.position() + (ball_.velocity() * dt));
    ball_.set_velocity(ball_.velocity() + (Vector3(0, (-10 * dt), 0)));

    //goal checks
    if ((ball_.position().x() <= (10 - ball_.radius()) && ball_.position().x() >= (-10 + ball_.radius())
        && ball_.position().y() <= (10 - ball_.radius()) && ball_.position().y() >= (0 + ball_.radius())
        && (ball_.position().z() >= (50 - ball_.radius()) || (ball_.position().z() <= (-50 + ball_.radius()))))) {
        
        ball_.Reset();
        car_.Reset();
    }


    // right wall ball check
    if (ball_.position().x() + ball_.radius() > 40){
        
        ball_.set_position(Point3(40-ball_.radius(), ball_.position().y(), ball_.position().z()));
        
        ball_.set_velocity(Vector3(-ball_.velocity().x() * 0.8, ball_.velocity().y() * 0.8, ball_.velocity().z() * 0.8));
    }

    // left wall ball check
    if (ball_.position().x() - ball_.radius() < -40) {

        ball_.set_position(Point3(-40 + ball_.radius(), ball_.position().y(), ball_.position().z()));

        ball_.set_velocity(Vector3(-ball_.velocity().x() * 0.8, ball_.velocity().y() * 0.8, ball_.velocity().z() * 0.8));
    }

    // ball floor check
    if (ball_.position().y() - ball_.radius() < 0) {
        
        ball_.set_position(Point3(ball_.position().x(), ball_.radius(), ball_.position().z()));

        ball_.set_velocity(Vector3(ball_.velocity().x() * 0.8, -ball_.velocity().y() * 0.8, ball_.velocity().z() * 0.8));
    }

    //  ball ceiling check
    if (ball_.position().y() + ball_.radius() > 35) {

        ball_.set_position(Point3(ball_.position().x(), 35 - ball_.radius(), ball_.position().z()));

        ball_.set_velocity(Vector3(ball_.velocity().x() * 0.8, -ball_.velocity().y() * 0.8, ball_.velocity().z() * 0.8));
    }
    
    // blue goal ball wall check
    if (ball_.position().z() + ball_.radius() > 50) {

        ball_.set_position(Point3(ball_.position().x(), ball_.position().y(), 50 - ball_.radius()));

        ball_.set_velocity(Vector3(ball_.velocity().x() * 0.8, ball_.velocity().y() * 0.8, -ball_.velocity().z() * 0.8));
    }
    
    // purple goal ball wall check
    if (ball_.position().z() - ball_.radius() < -50) {

        ball_.set_position(Point3(ball_.position().x(), ball_.position().y(), -50 + ball_.radius()));

        ball_.set_velocity(Vector3(ball_.velocity().x() * 0.8, ball_.velocity().y() * 0.8, -ball_.velocity().z() * 0.8));
    }

    // right wall car check
    if (car_.position().x() + car_.collision_radius() > 40) {

        car_.set_position(Point3(40 - car_.collision_radius(), car_.position().y(), car_.position().z()));

    }

    // left wall car check
    if (car_.position().x() - car_.collision_radius() < -40) {

        car_.set_position(Point3(-40 + car_.collision_radius(), car_.position().y(), car_.position().z()));
    
    }

    // car floor check
    if (car_.position().y() - car_.collision_radius() < 0) {

        car_.set_position(Point3(car_.position().x(), car_.collision_radius(), car_.position().z()));
        
    }

    //  car ceiling check
    if (car_.position().y() + car_.collision_radius() > 35) {

        car_.set_position(Point3(car_.position().x(), 35 - car_.collision_radius(), car_.position().z()));
        
    }

    // blue goal car wall check
    if (car_.position().z() + car_.collision_radius() > 50) {

        car_.set_position(Point3(car_.position().x(), car_.position().y(), 50 - car_.collision_radius()));
        
    }

    // purple goal car wall check
    if (car_.position().z() - car_.collision_radius() < -50) {

        car_.set_position(Point3(car_.position().x(), car_.position().y(), -50 + car_.collision_radius()));
        
    }
    // ball car collision equation
    

    Vector3 distance_vec = ball_.position() - car_.position();

    if (distance_vec.Length() < ball_.radius() + car_.collision_radius()) {
        // (ball_.velocity() - (2 * (ball_.velocity().Dot(distance_vec.ToUnit()))) * distance_vec.ToUnit());
        ball_.set_position(ball_.position() + (ball_.radius() + car_.collision_radius()) * distance_vec.ToUnit());
        Vector3 vrel = ball_.velocity() - car_.velocity();
        Vector3 vrel2 = (vrel - (2 * (vrel.Dot(distance_vec.ToUnit()))) * distance_vec.ToUnit());
        ball_.set_velocity(car_.velocity() + vrel2);

    }
    // reflecting a velocity around a normal equation
    // r (new velocity) = v - 2(v.n)n

    // vrel = vball - vcar
    // vball = vcar + vrel

    // Example: This is not the "correct way" to drive the car, but this code
    // will at least move the car around for testing
    float metersPerSec = 10.0f;
   
    /* You could store velocity in a Vector3.  The speed will be the magnitude or length of the velocity vector.  Since the car never moves up or down, the Y component of the vector 
    will always be 0 and the X and Y components will specify the car's direction or heading.  If you prefer to get the heading as an angle, you could calculate this as well.  
    To do that, I would compare the velocity vector with some reference vector. <0,0,-1> points in the car's initial forward direction, driving into the screen, so I would probably 
    think of the car's angle as equaling zero when it points in the <0,0,-1> direction.   If you take the dot product of the velocity vector with this reference vector, we know this
    will give us the cosine of the angle between the two vectors.  So, you could then find the actual angle between them with an arccos, something like acos(vel.dot(referenceVec)); */

    // car_.set_position(car_.position() + metersPerSec * Vector3(dpad_dir[0], 0, -dpad_dir[1]) * dt);
    
    // vector right control steering dir[0] -1 = left, 1 - right
    // starting forward = Vector3(0,0,-1)
    //sins cos
    float acc = 100.0f;
    float fsens = 0.05;

    if (car_.speed() < 0.5 || car_.speed() > -0.5) {
        car_.set_speed(car_.speed() + (dt * acc * dpad_dir[1]));
    }
    car_.set_speed(car_.speed() * 0.8);
    
    Matrix4 M = Matrix4::RotationY(GfxMath::ToDegrees(fsens * -dpad_dir[0] * dt));
    std::cout << (car_.speed());
    if (car_.speed() < -3.75 || car_.speed() > 3.75) {
        car_.set_forward(M * car_.forward());
    }
    car_.set_position(car_.position() + car_.velocity() * dt);
    // up down gas control dir[1] -1 = down, 1 = up speed float
    // starting speed = 0.0f
    // car_.set_speed();
    
}


void CarSoccer::InitOpenGL() {
    // Set up the camera in a good position to see the entire field
    projMatrix_ = Matrix4::Perspective(60, aspect_ratio(), 1, 1000);
    modelMatrix_ = Matrix4::LookAt(Point3(0,60,70), Point3(0,0,10), Vector3(0,1,0));
 
    // Set a background color for the screen (don't worry if you get a depricated warning on this line in OSX)
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    
    // Load some image files we'll use
    fieldTex_.InitFromFile(Platform::FindFile("pitch.png", searchPath_));
    crowdTex_.InitFromFile(Platform::FindFile("crowd.png", searchPath_));
}


void CarSoccer::DrawUsingOpenGL() {
    // Draw the crowd as a fullscreen background image
    quickShapes_.DrawFullscreenTexture(Color(1,1,1), crowdTex_);

    // Draw the car and the ball
    car_.Draw(quickShapes_, modelMatrix_, viewMatrix_, projMatrix_);
    ball_.Draw(quickShapes_, modelMatrix_, viewMatrix_, projMatrix_);
    // quickShapes_.DrawArrow(modelMatrix_, viewMatrix_, projMatrix_, Color(1, 0, 1), car_.position(), car_.velocity()*10.0f, 0.1f);
    
    // Draw the field with the field texture on it.
    Color col(16.0f/255.0f, 46.0f/255.0f, 9.0f/255.0f);
    Matrix4 M = Matrix4::Translation(Vector3(0.0f, -0.201f, 0.0f)) * Matrix4::Scale(Vector3(50.0f, 1.0f, 60.0f));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, col);
    M = Matrix4::Translation(Vector3(0.0f, -0.2f, 0.0f)) * Matrix4::Scale(Vector3(40.0f, 1.0f, 50.0f));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, Color(1,1,1), fieldTex_);
    
    // You should add drawing the goals and the boundary of the playing area
    // using quickShapes_.DrawLines()
    
    // floor boundary
    std::vector<Point3> floorp;
    floorp.push_back(Point3(-40.0, 0.0, 50.0));
    floorp.push_back(Point3(-40.0, 0.0, -50.0));
    floorp.push_back(Point3(40.0, 0.0, -50.0));
    floorp.push_back(Point3(40.0, 0.0, 50.0));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, Color(1, 1, 1), floorp, QuickShapes::LinesType::LINE_LOOP, 0.1);

    // ceiling boundary
    std::vector<Point3> ceilingp;
    ceilingp.push_back(Point3(-40.0, 35.0, 50.0));
    ceilingp.push_back(Point3(-40.0, 35.0, -50.0));
    ceilingp.push_back(Point3(40.0, 35.0, -50.0));
    ceilingp.push_back(Point3(40.0, 35.0, 50.0));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, Color(1, 1, 1), ceilingp, QuickShapes::LinesType::LINE_LOOP, 0.1);

    //corners
    std::vector<Point3> nect;
    nect.push_back(Point3(-40.0, 0.0, 50.0));
    nect.push_back(Point3(-40.0, 35.0, 50.0));
    nect.push_back(Point3(-40.0, 0.0, -50.0));
    nect.push_back(Point3(-40.0, 35.0, -50.0));
    nect.push_back(Point3(40.0, 0.0, -50.0));
    nect.push_back(Point3(40.0, 35.0, -50.0));
    nect.push_back(Point3(40.0, 0.0, 50.0));
    nect.push_back(Point3(40.0, 35.0, 50.0));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, Color(1, 1, 1), nect, QuickShapes::LinesType::LINES, 0.1);
    
    // purple lower team goal cords
    std::vector<Point3> pgoal;
    pgoal.push_back(Point3(-10.0, 0.0, 50));
    pgoal.push_back(Point3(10.0, 0.0, 50));
    pgoal.push_back(Point3(10.0, 10.0, 50));
    pgoal.push_back(Point3(-10.0, 10.0, 50));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, Color(1, 0, 0.75), pgoal, QuickShapes::LinesType::LINE_LOOP, 0.1);
    
    std::vector<Point3> pmesh;
    for (float i = 0; i <= 10; i++) {
        for (float j = 0; j <= 20; j++) {
            pmesh.push_back(Point3(j - 10, i, 50));
            pmesh.push_back(Point3(j - 10, 10-i, 50));
            pmesh.push_back(Point3(-10, i, 50));
            pmesh.push_back(Point3(10, i, 50));
        }
    }
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, Color(1, 0, 0.75), pmesh, QuickShapes::LinesType::LINES, 0.1);

    // blue upper team goal cords
    std::vector<Point3> bgoal;
    bgoal.push_back(Point3(-10.0, 0.0, -50));
    bgoal.push_back(Point3(10.0, 0.0, -50));
    bgoal.push_back(Point3(10.0, 10.0, -50));
    bgoal.push_back(Point3(-10.0, 10.0, -50));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, Color(0, 0, 1), bgoal, QuickShapes::LinesType::LINE_LOOP, 0.1);
    
    std::vector<Point3> bmesh;
    for (float i = 0; i <= 10; i++) {
        for (float j = 0; j <= 20; j++) {
            bmesh.push_back(Point3(j - 10, i, -50));
            bmesh.push_back(Point3(j - 10, 10 - i, -50));
            bmesh.push_back(Point3(-10, i, -50));
            bmesh.push_back(Point3(10, i, -50));
        }
    }
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, Color(0, 0, 1), bmesh, QuickShapes::LinesType::LINES, 0.1);
}


