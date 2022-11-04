/** CSci-4611 In-Class Example */

#ifndef EXAMPLE_H_
#define EXAMPLE_H_

#include <mingfx.h>
using namespace mingfx;

#include <string>
#include <vector>

class Example : public GraphicsApp {
public:
  
    // Creates the App
    Example();
    
    // Cleans up when the App shuts down
    virtual ~Example();
    
    // Note a Run() function is inherited from GraphicsApp, that's what
    // actually starts up the App.
    
    void OnLeftMouseDown(const Point2& pos);
    void OnLeftMouseDrag(const Point2& pos, const Vector2& delta);
    void OnLeftMouseUp(const Point2& pos);

    void OnRightMouseDrag(const Point2& pos, const Vector2& delta);

    // This gets called once each frame.  Note that dt (a.k.a., "delta time") is
    // the amount of time (in seconds) that has passed since the last frame.
    void UpdateSimulation(double dt);
    
    // This is where we initialize any OpenGL data, like textures or meshes that
    // need to be loaded from files and setup in OpenGL.  It gets called once
    // when the program starts up.
    void InitOpenGL();
    
    // This gets called once each frame, and this is where you draw the latest
    // version of your 3D graphics scene.
    void DrawUsingOpenGL();
    
    
private:
    
    // This implements something like the camera controls in minecraft.  You can
    // walk around using the arrow keys or ASWZ keys.  You can also move your
    // head to look around by dragging with the right mouse button.  Internally,
    // the camera creates a view matrix.
    CraftCam cam_;
    Matrix4 proj_matrix_;
    
    // A helper class for drawing some simple shapes (cubes, spheres, 3D arrows)
    QuickShapes quick_shapes_;
 
    // A list of paths to search for data files (images and shaders)
    std::vector<std::string> search_path_;
   
    Texture2D dart_tex_;
 };

#endif