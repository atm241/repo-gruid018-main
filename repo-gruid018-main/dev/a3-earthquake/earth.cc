/** CSci-4611 Assignment 3:  Earthquake
 */

#include "earth.h"
#include "config.h"

#include <vector>

// for M_PI constant
#define _USE_MATH_DEFINES
#include <math.h>


Earth::Earth() {
    std::vector<Point3> vertices;
    std::vector<Vector3> normals;
    std::vector<Point3> verticeSphere;
    std::vector<Vector3> normalSphere;
    std::vector<Point3> verticesPlane;
    std::vector<Vector3> normalsPlane;
    
}

Earth::~Earth() {
}

void Earth::Init(const std::vector<std::string>& search_path) {
    // init shader program
    shader_.Init();

    // init texture: you can change to a lower-res texture here if needed
    earth_tex_.InitFromFile(Platform::FindFile("earth-1k.png", search_path));

    // init geometry
    const int nslices = 20;
    const int nstacks = 20;

    // TODO: This is where you need to set the vertices and indiceds for earth_mesh_.
    
    // As a demo, we'll add a square with 2 triangles.
    
    std::vector<unsigned int> indices;
    std::vector<Point2> tex_coords;

    for (int i = 0; i <= nstacks; i++) {
        for (int j = 0; j <= nslices; j++) {
            float u = (float)j;
            float v = (float)i;

            float vertLong = (-M_PI + (j * ((2 * M_PI) / nslices)));
            float vertLat = ((-M_PI / 2) + (i * (M_PI / nstacks)));


            vertices.push_back(LatLongToPlane(vertLat, vertLong));
            verticeSphere.push_back(LatLongToSphere(vertLat, vertLong));
            verticesPlane.push_back(LatLongToPlane(vertLat, vertLong));

            //fix sphere normals for light
            normalSphere.push_back(Point3(0,0,0) - (LatLongToSphere(vertLat, vertLong)));
            normalsPlane.push_back(Vector3(0, 0, 1));
            normals.push_back(Vector3(0, 0, 1));
            tex_coords.push_back(Point2(u / nslices, 1.0 - (v / nstacks)));
        }
    }
    
    
    for (int i = 0; i < nstacks; i++) {
        for (int j = 0; j < nslices; j++) {
            int stackz = i;
            int slicez = j;
            indices.push_back(slicez + (stackz * (nslices + 1)));
            
            indices.push_back((slicez + ((stackz + 1) * (nslices + 1)))+1);
            indices.push_back(slicez + ((stackz + 1) * (nslices + 1)));
            
            indices.push_back(slicez + (stackz * (nslices + 1)));
            indices.push_back((slicez + (stackz * (nslices + 1)))+1);
            indices.push_back((slicez + ((stackz + 1) * (nslices + 1))) + 1);
            
            /* indices.push_back();
            indices.push_back( + nslices + 2);
            indices.push_back( + nslices + 1);
        */
        }
        
    }
  
    earth_mesh_.SetVertices(vertices);
    earth_mesh_.SetNormals(normals);
    earth_mesh_.SetIndices(indices);
    earth_mesh_.SetTexCoords(0, tex_coords);
    earth_mesh_.UpdateGPUMemory();
}



void Earth::Draw(const Matrix4 &model_matrix, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {
    // Define a really bright white light.  Lighting is a property of the "shader"
    DefaultShader::LightProperties light;
    light.position = Point3(30,30,30);
    light.ambient_intensity = Color(1,1,1);
    light.diffuse_intensity = Color(1,1,1);
    light.specular_intensity = Color(1,1,1);
    shader_.SetLight(0, light);

    // Adust the material properties, material is a property of the thing
    // (e.g., a mesh) that we draw with the shader.  The reflectance properties
    // affect the lighting.  The surface texture is the key for getting the
    // image of the earth to show up.
    DefaultShader::MaterialProperties mat;
    mat.ambient_reflectance = Color(0.5, 0.5, 0.5);
    mat.diffuse_reflectance = Color(0.75, 0.75, 0.75);
    mat.specular_reflectance = Color(0.75, 0.75, 0.75);
    mat.surface_texture = earth_tex_;

    // Draw the earth mesh using these settings
    if (earth_mesh_.num_triangles() > 0) {
        shader_.Draw(model_matrix, view_matrix, proj_matrix, &earth_mesh_, mat);
    }
}


Point3 Earth::LatLongToSphere(double latitude, double longitude) const {
    // TODO: We recommend filling in this function to put all your
    // lat,long --> sphere calculations in one place.
   // double latRad = GfxMath::ToRadians(latitude);
   // double longRad = GfxMath::ToRadians(longitude);
   double x = cos(latitude) * sin(longitude);
   double y = sin(latitude);
   double z = cos(latitude) * cos(longitude);
    return Point3(x,y,z);
}

Point3 Earth::LatLongToPlane(double latitude, double longitude) const {
    // TODO: We recommend filling in this function to put all your
   // lat,long --> plane calculations in one place.
    return Point3(longitude,latitude,0);
}

void Earth::DrawGlobeMode(bool globe_mode) {
    //failed lerp
    /* float alpha = 0.0;
    if (globe_mode && alpha < 1.0) {
        alpha = alpha + 0.01;
    }
    if (!globe_mode && alpha > 0.0) {
        alpha = alpha - 0.01;
    }
    
    for (int i = 0; i < vertices.size(); i++) {
            vertices[i] = Point3::Lerp(verticesPlane[i], verticeSphere[i], alpha);
            normals[i] = Vector3::Lerp(normalsPlane[i], normalSphere[i], alpha);
        }
         vertices.push_back(Earth::LatLongToSphere(vertLat,vertLong));
     */  
        // old code for non lerp if else
    if(globe_mode){
        for (int i = 0; i < vertices.size(); i++) {
            vertices[i] = verticeSphere[i];
            normals[i] = normalSphere[i];
        }
    } else {
        for (int i = 0; i < vertices.size(); i++) {
            vertices[i] = verticesPlane[i];
            normals[i] = normalsPlane[i];
        }
    }
    
    earth_mesh_.SetVertices(vertices);
    earth_mesh_.SetNormals(normals);
    earth_mesh_.UpdateGPUMemory();
    

}

void Earth::DrawDebugInfo(const Matrix4 &model_matrix, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {
    // This draws a cylinder for each line segment on each edge of each triangle in your mesh.
    // So it will be very slow if you have a large mesh, but it's quite useful when you are
    // debugging your mesh code, especially if you start with a small mesh.
    for (int t=0; t<earth_mesh_.num_triangles(); t++) {
        std::vector<unsigned int> indices = earth_mesh_.read_triangle_indices_data(t);
        std::vector<Point3> loop;
        loop.push_back(earth_mesh_.read_vertex_data(indices[0]));
        loop.push_back(earth_mesh_.read_vertex_data(indices[1]));
        loop.push_back(earth_mesh_.read_vertex_data(indices[2]));
        quick_shapes_.DrawLines(model_matrix, view_matrix, proj_matrix,
            Color(1,1,0), loop, QuickShapes::LinesType::LINE_LOOP, 0.005f);
    }
}

