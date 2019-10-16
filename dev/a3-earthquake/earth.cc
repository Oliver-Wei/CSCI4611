/** CSci-4611 Assignment 3:  Earthquake
 */

#include "earth.h"
#include "config.h"

#include <vector>

// for M_PI constant
#define _USE_MATH_DEFINES
#include <math.h>


Earth::Earth() {
}

Earth::~Earth() {
}

float lerp(float x, float y, float a){
    return x + a*(y-x);
}

void Earth::Init(const std::vector<std::string> &search_path) {
    // init shader program
    shader_.Init();
    
    // init texture: you can change to a lower-res texture here if needed
    earth_tex_.InitFromFile(Platform::FindFile("earth-2k.png", search_path));

    // init geometry
    const int nslices = 18;
    const int nstacks = 9;

    // TODO: This is where you need to set the vertices and indiceds for earth_mesh_.

    // As a demo, we'll add a square with 2 triangles.
    
    for (int i=0; i < nslices+1; i++){
        for (int j = 0; j < nstacks+1; j++){
            vertices.push_back(Point3(lerp(-M_PI,M_PI,i * 1.0/nslices),lerp(-M_PI/2,M_PI/2,j * 1.0/nstacks),0));
            sphere_vertices.push_back(LatLongToSphere(lerp(-M_PI/2,M_PI/2,j * 1.0/nstacks),lerp(-M_PI,M_PI,i * 1.0/nslices)));
            tex_coords.push_back(Point2(1-i * 1.0/nslices,1-j * 1.0/nstacks));
//            std::vector<Vector3> normals;
        }
    }
//    for (int i=0; i < nslices+1; i++){
//        for (int j = 0; j < nstacks+1; j++){

    for (int i = 0; i < nslices;i++){
        for (int j = 0; j < nstacks; j++){
            indices.push_back(0+j+(nstacks+1)*i);
            indices.push_back(nstacks+1+j+(nstacks+1)*i);
            indices.push_back(1+j+(nstacks+1)*i);
        }
    }
    
    for (int i = 0; i < nslices;i++){
        for (int j = 0; j < nstacks; j++){
            indices.push_back(1+j+(nstacks+1)*i);
            indices.push_back(nstacks+1+j+(nstacks+1)*i);
            indices.push_back(nstacks+2+j+(nstacks+1)*i);
        }

    }
    
    earth_mesh_.SetVertices(vertices);
    earth_mesh_.SetIndices(indices);
    earth_mesh_.SetTexCoords(0, tex_coords);
    earth_mesh_.UpdateGPUMemory();
}



void Earth::Draw(const Matrix4 &model_matrix, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {
    // Define a really bright white light.  Lighting is a property of the "shader"
    DefaultShader::LightProperties light;
    light.position = Point3(10,10,10);
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
    float x = cos(latitude) * sin(longitude);
    float y = sin(latitude);
    float z = cos(latitude)*cos(longitude);
    
    return Point3(x,y,z);
}

Point3 Earth::LatLongToPlane(double latitude, double longitude) const {
    // TODO: We recommend filling in this function to put all your
    // lat,long --> plane calculations in one place.
    
    return Point3(longitude,latitude,0);
}



void Earth::DrawDebugInfo(const Matrix4 &model_matrix, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {
    // This draws a cylinder for each line segment on each edge of each triangle in your mesh.
    // So it will be very slow if you have a large mesh, but it's quite useful when you are
    // debugging your mesh code, especially if you start with a small mesh.
    for (int t=0; t<earth_mesh_.num_triangles(); t++) {
        std::vector<unsigned int> indices = earth_mesh_.triangle_vertices(t);
        std::vector<Point3> loop;
        loop.push_back(earth_mesh_.vertex(indices[0]));
        loop.push_back(earth_mesh_.vertex(indices[1]));
        loop.push_back(earth_mesh_.vertex(indices[2]));
        quick_shapes_.DrawLines(model_matrix, view_matrix, proj_matrix,
            Color(1,1,0), loop, QuickShapes::LinesType::LINE_LOOP, 0.005);
    }
}

void Earth::Update(bool global_mode){
    if (global_mode){
        earth_mesh_.SetVertices(sphere_vertices);
        earth_mesh_.SetIndices(indices);
        earth_mesh_.SetTexCoords(0, tex_coords);
        earth_mesh_.UpdateGPUMemory();
    }
    else{
        earth_mesh_.SetVertices(vertices);
        earth_mesh_.SetIndices(indices);
        earth_mesh_.SetTexCoords(0, tex_coords);
        earth_mesh_.UpdateGPUMemory();
    }

}
