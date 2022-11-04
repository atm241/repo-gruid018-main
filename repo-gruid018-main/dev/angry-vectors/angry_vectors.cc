/** CSci-4611 In-Class Example */

#include "angry_vectors.h"

#include <iostream>
#include <sstream>



AngryVectors::AngryVectors() : GraphicsApp(1024,768, "AngryVectors"), time_(0.0), playing_(true), pauseBtn_(NULL) {
}



AngryVectors::~AngryVectors() {
}



void AngryVectors::OnPauseBtnPressed() {
    playing_ = !playing_;
    if (playing_) {
        pauseBtn_->setCaption("Pause");
    }
    else {
        pauseBtn_->setCaption("Play");
    }
}


Point3 AngryVectors::CalcBirdPos(double t) {
    // projectile motion for the "bird"
    double e = t / 4.0;
    double x = e * 52.0 - 30.0;
    double y = 30.0 * (-e * e + 1.2 * e) + 5.0;
    double z = -35.0;
    return Point3((float)x, (float)y, (float)z);
}

Vector3 AngryVectors::CalcBirdVel(double t) {
    return CalcBirdPos(time_+0.1) - CalcBirdPos(time_);
}


void AngryVectors::UpdateSimulation(double dt)  {
    if (playing_) {
        time_ += (float)dt;
        if (time_ > 4) {
            time_ = 0.0;
        }
    }
}



void AngryVectors::InitNanoGUI() {
    // Setup the GUI window
    nanogui::Window *window = new nanogui::Window(screen(), "Controls");
    window->setPosition(Eigen::Vector2i(10, 10));
    window->setSize(Eigen::Vector2i(400,200));
    window->setLayout(new nanogui::GroupLayout());
    
    pauseBtn_ = new nanogui::Button(window, "Pause");
    pauseBtn_->setCallback(std::bind(&AngryVectors::OnPauseBtnPressed, this));
    pauseBtn_->setTooltip("Toggle playback.");
    
    screen()->performLayout();
}



void AngryVectors::InitOpenGL() {
    // Set up the camera in a good position to see the entire scene
    proj_matrix_ = Matrix4::Perspective(60.0f, aspect_ratio(), 0.01f, 100.0f);
    view_matrix_ = Matrix4::LookAt(Point3(0,2,15), Point3(0,0,0), Vector3(0,1,0));
    glClearColor(0.2f, 0.6f, 1.0f, 1.0f);
}


void AngryVectors::DrawUsingOpenGL() {
    // ground
    Matrix4 ground_matrix = Matrix4::Translation(Vector3(0,-2,-50)) * Matrix4::Scale(Vector3(80, 2, 100));
    quick_shapes_.DrawCube(ground_matrix, view_matrix_, proj_matrix_, Color(0.3f, 0.9f, 0.4f));
  
    // 3 targets
    Matrix4 target1_matrix = Matrix4::Translation(Vector3(21, 6, -35)) * Matrix4::Scale(Vector3(1.5, 6, 10));
    quick_shapes_.DrawCube(target1_matrix, view_matrix_, proj_matrix_, Color(0.6f, 0.4f, 0.2f));

    Matrix4 target2_matrix = Matrix4::Translation(Vector3(25, 6, -35)) * Matrix4::Scale(Vector3(1.5, 6, 10));
    quick_shapes_.DrawCube(target2_matrix, view_matrix_, proj_matrix_, Color(0.6f, 0.4f, 0.2f));

    Matrix4 target3_matrix = Matrix4::Translation(Vector3(23, 18, -35)) * Matrix4::Scale(Vector3(1.5, 6, 10));
    quick_shapes_.DrawCube(target3_matrix, view_matrix_, proj_matrix_, Color(0.6f, 0.4f, 0.2f));

    // launcher
    Matrix4 launcher_matrix;
    quick_shapes_.DrawLineSegment(launcher_matrix, view_matrix_, proj_matrix_, Color(0,0,0),
                                  Point3(-30,-2,-35), Point3(-30, 5, -35), 0.75);
    
    // bird
    Point3 bird_pos = CalcBirdPos(time_);
    Matrix4 bird_matrix = Matrix4::Translation(bird_pos - Point3::Origin());
    quick_shapes_.DrawSphere(bird_matrix, view_matrix_, proj_matrix_, Color(1,0,0));
    
    
    
    // DEBUGGING OUTPUT
    
    // this is useful to be able to see where the origin is and the scale.  The
    // axes are drawn at (0,0,0), and each arrow is 1 unit long.  The red axis is X,
    // green is Y, and blue is Z.
    Matrix4 identity;
    quick_shapes_.DrawAxes(identity, view_matrix_, proj_matrix_);
    
    
    // TODO: make sure bird velocity makes sense. (hint, hint)

   

}





