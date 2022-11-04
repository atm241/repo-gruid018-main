/** CSci-4611 In-Class Example */

#include "example.h"

#include "config.h"

#include <iostream>
#include <sstream>



Example::Example() : GraphicsApp(1024,768, "MinGfx Example") {
    // Define a search path for finding data files (images and shaders)
    search_path_.push_back(".");
    search_path_.push_back("./data");
    search_path_.push_back(DATA_DIR_INSTALL);
    search_path_.push_back(DATA_DIR_BUILD);
}


Example::~Example() {
}


void Example::UpdateSimulation(double dt)  {
    cam_.UpdateSimulation(dt, window());
}

// You can look around, like in minecraft, by dragging with the right mouse button.
void Example::OnRightMouseDrag(const Point2& mouse_in_pixels, const Vector2& delta_in_pixels) {
    Vector2 delta_in_ndc = PixelsToNormalizedDeviceCoords(delta_in_pixels);
    cam_.OnMouseMove(delta_in_ndc);
}




void Example::OnLeftMouseDown(const Point2& mouse_in_pixels) {

}

void Example::OnLeftMouseDrag(const Point2& mouse_in_pixels, const Vector2& delta_in_pixels) {

}

void Example::OnLeftMouseUp(const Point2& mouse_in_pixels) {

}






void Example::InitOpenGL() {
    // Set up the camera in a good position to see the entire scene
    proj_matrix_ = Matrix4::Perspective(60.0f, aspect_ratio(), 0.01f, 100.0f);
    cam_.set_view_matrix(Matrix4::LookAt(Point3(0,0,3), Point3(0,0,0), Vector3(0,1,0)));
    glClearColor(1,1,1,1);
 
    // load textures and shaders
    dart_tex_.InitFromFile(Platform::FindFile("dartboard.jpg", search_path_));
}


void Example::DrawUsingOpenGL() {
    Matrix4 view_matrix = cam_.view_matrix();

    // draws a set of axes at the world origin, since we are passing the identity
    // matrix for the "model" matrix.
    Matrix4 identity;
    quick_shapes_.DrawAxes(identity, view_matrix, proj_matrix_);

    Matrix4 boardTransform = Matrix4::RotationX(GfxMath::ToRadians(90));
    quick_shapes_.DrawSquare(boardTransform, view_matrix, proj_matrix_, Color(1, 1, 1), dart_tex_);
}




