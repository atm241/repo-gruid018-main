/** CSci-4611 In-Class Example */

#include "snowman.h"

#include <iostream>
#include <sstream>



Snowman::Snowman() : GraphicsApp(1024,768, "Do You Want to Build a Snowman?") {
}


Snowman::~Snowman() {
}


void Snowman::UpdateSimulation(double dt)  {
}


void Snowman::InitOpenGL() {
    // Set up the camera in a good position to see the entire scene
    proj_matrix_ = Matrix4::Perspective(60.0f, aspect_ratio(), 0.01f, 100.0f);
    view_matrix_ = Matrix4::LookAt(Point3(0,2,10), Point3(0,0,0), Vector3(0,1,0));
    glClearColor(0.2f, 0.6f, 1.0f, 1.0f);
}


void Snowman::DrawUsingOpenGL() {
    Matrix4 identity;
    
    // draws a set of axes at the world origin, since we are passing the identity
    // matrix for the "model" matrix.
    quick_shapes_.DrawAxes(identity, view_matrix_, proj_matrix_);
    

    Matrix4 S_ground = Matrix4::Scale(Vector3(40.0f, 0.2f, 40.0f));
    Matrix4 T_ground = Matrix4::Translation(Vector3(0.0f, -0.2f, 0.0f));
    Matrix4 ground_combo = T_ground * S_ground;
    quick_shapes_.DrawCube(ground_combo, view_matrix_, proj_matrix_, Color(0.8f, 0.8f, 0.8f));

}





