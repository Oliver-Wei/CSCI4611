# CSci-4611 Assignment 3:  Earthquake

Yingxin Wei

1. Set the vertices and indices for earth_mesh_

I use a rectangle subdivided into multiple triangles to represent a flat map of the earth.The flat map mesh lie in the xy plane. It has x values ranging from −π to π nad y values ranging from −π/2 to π/2. It is divided into nslices divisions horizontally and nstacks divisions vertically. The 3D coordiantes in the plane view, 3D coordinates in the global view, texture coordinates and normals in both views of all vertices are calculated in the for loops.

    float lerp(float x, float y, float a){
        return x + a*(y-x);
    }

    for (int i=0; i < nslices+1; i++){
        for (int j = 0; j < nstacks+1; j++){
            Point3 sphere_point = Point3(LatLongToSphere(lerp(-M_PI/2,M_PI/2,j * 1.0/nstacks),lerp(-M_PI,M_PI,i * 1.0/nslices)));
            Point3 plane_point = Point3(LatLongToPlane(lerp(-M_PI,M_PI,i * 1.0/nslices),lerp(-M_PI/2,M_PI/2,j * 1.0/nstacks)));
            vertices.push_back(plane_point);
            sphere_vertices.push_back(sphere_point);
            tex_coords.push_back(Point2(i * 1.0/nslices,1-j * 1.0/nstacks));
            sphere_normals.push_back(sphere_point - Point3(0,0,0));
            normals.push_back(Vector3(0,0,1));
        }
    }

The 3D Cartesian coordinates of the corresponding point on the plane can be calculated using LatLongToPlane(latitude_, longitude), while the 3D Cartesian coordinates of the corresponding point on the sphere can be calculated using LatLongToSphere(latitude_, longitude).

    Point3 Earth::LatLongToPlane(double latitude, double longitude) const {
        // TODO: We recommend filling in this function to put all your
        // lat,long --> plane calculations in one place.
        return Point3(longitude,latitude,0);
    }

    Point3 Earth::LatLongToSphere(double latitude, double longitude) const {
        // TODO: We recommend filling in this function to put all your
        // lat,long --> sphere calculations in one place.
        float x = cos(latitude) * sin(longitude);
        float y = sin(latitude);
        float z = cos(latitude)*cos(longitude);
        
        return Point3(x,y,z);
    }


The indices of the vertices are also calculated in a few for loops and are set by using earth_mesh_.SetIndices(indices).

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

In the plane view, the 3D coordinates, texture coordinates and normals of these vertices  and indices are set by using earth_mesh_.SetVertices(vertices), earth_mesh_.SetTexCoords(0, tex_coords),earth_mesh_.SetNormals(normals) and earth_mesh_.SetIndices(indices).

    earth_mesh_.SetVertices(vertices);
    earth_mesh_.SetIndices(indices);
    earth_mesh_.SetTexCoords(0, tex_coords);
    earth_mesh_.SetNormals(normals);
    earth_mesh_.UpdateGPUMemory();

In the global view, the 3D coordinates and normals of these vertices will be updated by using earth_mesh_.SetVertices(sphere_vertices) and earth_mesh_.SetNormals(sphere_normals), but we don't need to change their texture coordinates and indices.

    earth_mesh_.SetVertices(sphere_vertices);
    earth_mesh_.SetIndices(indices);
    earth_mesh_.SetTexCoords(0, tex_coords);
    earth_mesh_.SetNormals(sphere_normals);
    earth_mesh_.UpdateGPUMemory();

2. Draw Earthquake

To find the earthquakes that have happened in the past one year of the current visualization time, the program would firstly find the correct time one year ago by subtracting PLAYBACK_WINDOW(Number of seconds in 1 year (approx.)) from the current_time. Then I can find the start index (included) of the earthquakes in the past one year by finding the most recent quake after the one_year_ago_time_ and find the end index (excluded) by finding the most recent quake after current_time_.

    double one_year_ago_time_ = current_time_ - PLAYBACK_WINDOW;  // seconds
    int earthquake_start_index = quake_db_.FindMostRecentQuake(Date(one_year_ago_time_));
    int earthquake_end_index = quake_db_.FindMostRecentQuake(Date(current_time_));

Then the program will draw each earthquake between this two indices. After the longitude and the latitude of the earthquake is found, the position of the earthquake in the sphere is mapped by using LatLongToSphere(GfxMath::ToRadians(latitude_), GfxMath::ToRadians(longitude_)) if the earth is in the global view. Normalize the magnitude of the quake and draw the quake at corresponding position using the sphere whose red channel value and size is proportional to the magnitude.

    for (int i = earthquake_start_index; i < earthquake_end_index; i++){
        double longitude_ = quake_db_.earthquake(i).longitude();
        double latitude_ = quake_db_.earthquake(i).latitude();
        double scaled_magnitude_ = (quake_db_.earthquake(i).magnitude()-quake_db_.min_magnitude())/(quake_db_.max_magnitude() - quake_db_.min_magnitude());
        Color earthquake(scaled_magnitude_, 0.7, 0);
        Point3 sphere_position = global_model_matrix*earth_.LatLongToSphere(GfxMath::ToRadians(latitude_), GfxMath::ToRadians(longitude_));
        Matrix4 Mquakes =
            Matrix4::Translation(sphere_position - Point3(0,0,0)) *
            Matrix4::Scale(Vector3(scaled_magnitude_*0.05, scaled_magnitude_*0.05, scaled_magnitude_*0.05));
        quick_shapes_.DrawSphere(model_matrix * Mquakes, view_matrix_, proj_matrix_, earthquake);
    }

    Point3 Earth::LatLongToSphere(double latitude, double longitude) const {
        // TODO: We recommend filling in this function to put all your
        // lat,long --> sphere calculations in one place.
        float x = cos(latitude) * sin(longitude);
        float y = sin(latitude);
        float z = cos(latitude)*cos(longitude);
        
        return Point3(x,y,z);
    }

If the earth is in the plane view, the position of the earthquake in the plane is mapped by using earth_.LatLongToPlane(GfxMath::ToRadians(latitude_), GfxMath::ToRadians(longitude_)). Normalize the magnitude of the quake and draw the quake at corresponding position using the sphere whose red channel value and size is proportional to the magnitude.

    for (int i = earthquake_start_index; i < earthquake_end_index; i++){
        double longitude_ = quake_db_.earthquake(i).longitude();
        double latitude_ = quake_db_.earthquake(i).latitude();
        double scaled_magnitude_ = (quake_db_.earthquake(i).magnitude()-quake_db_.min_magnitude())/(quake_db_.max_magnitude() - quake_db_.min_magnitude());
        Color earthquake(scaled_magnitude_, 0.7, 0);
        Point3 plane_position = earth_.LatLongToPlane(GfxMath::ToRadians(latitude_), GfxMath::ToRadians(longitude_));
        Matrix4 Mquakes =
            Matrix4::Translation(plane_position - Point3(0,0,0)) *
            Matrix4::Scale(Vector3(scaled_magnitude_*0.05, scaled_magnitude_*0.05, scaled_magnitude_*0.05));
        quick_shapes_.DrawSphere(model_matrix * Mquakes, view_matrix_, proj_matrix_, earthquake);
    }

    Point3 Earth::LatLongToPlane(double latitude, double longitude) const {
        // TODO: We recommend filling in this function to put all your
        // lat,long --> plane calculations in one place.
        return Point3(longitude,latitude,0);
    }

If it's in the transistion state between global mode and plane mode, the position of the earthquake is somewhere between two corresponding positions.

    for (int i = earthquake_start_index; i < earthquake_end_index; i++){
        double longitude_ = quake_db_.earthquake(i).longitude();
        double latitude_ = quake_db_.earthquake(i).latitude();
        double scaled_magnitude_ = (quake_db_.earthquake(i).magnitude()-quake_db_.min_magnitude())/(quake_db_.max_magnitude() - quake_db_.min_magnitude());
        Color earthquake(scaled_magnitude_, 0.7, 0);
        Point3 sphere_position = global_model_matrix*earth_.LatLongToSphere(GfxMath::ToRadians(latitude_), GfxMath::ToRadians(longitude_));
        Point3 plane_position = earth_.LatLongToPlane(GfxMath::ToRadians(latitude_), GfxMath::ToRadians(longitude_));
        Point3 transition_position = plane_position.Lerp(sphere_position,alpha);
        Matrix4 Mquakes =
            Matrix4::Translation(transition_position - Point3(0,0,0)) *
            Matrix4::Scale(Vector3(scaled_magnitude_*0.05, scaled_magnitude_*0.05, scaled_magnitude_*0.05));
        quick_shapes_.DrawSphere(model_matrix * Mquakes, view_matrix_, proj_matrix_, earthquake);
    }

3. Transform between global mode and plane mode, smooth morph

Once the Globe button is pressed, I will set a flag to realize smooth morph by repeatedly calling Earth::UpdateEarthMesh() for alpha between 0 to 1 and drawing many scenes between global and plane modes. Once the smooth morph (update) is finished (alpha is 0 or 1), the flag will be set to false, and then the mesh will not change and stay on global mode or plane mode.


    void QuakeApp::OnGlobeBtnPressed() {
        // TODO: This is where you can switch between flat earth mode and globe mode
        global_mode_ = !global_mode_;
        flag = true;
        if (global_mode_){
            globe_btn_-> setCaption("Map");
            global_model_matrix = model_matrix;
        }
        else{
            globe_btn_-> setCaption("Globe");
        }
    }

    bool Earth::UpdateEarthMesh(Matrix4 rotation_matrix, bool flag, float alpha){
        if (alpha == 0){
            earth_mesh_.SetVertices(vertices);
            earth_mesh_.SetIndices(indices);
            earth_mesh_.SetTexCoords(0, tex_coords);
            earth_mesh_.SetNormals(normals);
            earth_mesh_.UpdateGPUMemory();
            flag = false;
        }
        else if (alpha == 1){
            earth_mesh_.SetVertices(sphere_vertices);
            earth_mesh_.SetIndices(indices);
            earth_mesh_.SetTexCoords(0, tex_coords);
            earth_mesh_.SetNormals(sphere_normals);
            earth_mesh_.UpdateGPUMemory();
            flag = false;
        }
        else {
            std::vector<Point3> transition_vertices;
            std::vector<Vector3> transition_normals;
            for (int i = 0; i < vertices.size(); i++){
                transition_vertices.push_back( vertices.at(i).Lerp(rotation_matrix*(sphere_vertices.at(i)),alpha));
            transition_normals.push_back(rotation_matrix*(normals.at(i)).Lerp(sphere_normals.at(i),alpha));
            }
            earth_mesh_.SetVertices(transition_vertices);
            earth_mesh_.SetIndices(indices);
            earth_mesh_.SetTexCoords(0, tex_coords);
            earth_mesh_.SetNormals(transition_normals);
            earth_mesh_.UpdateGPUMemory();
        }
        return flag;
    }

In QuakeApp::UpdateSimulation(double dt), alpha will be repeatedly updated once the view is in transition state and earth_.UpdateEarthMesh() will be repeatedly called to update the mesh. Once the transition process is completed from plane mode to global mode, the global model matrix will rotate around the earth's axis according to the elapsed time.

    if (flag){
        if (global_mode_){
            alpha += 0.02;
        }
        else{
            alpha -= 0.02;
        }
        alpha = GfxMath::Clamp(alpha, 0, 1);
        flag = earth_.UpdateEarthMesh(global_model_matrix, flag, alpha);
    }
    else{
        if (global_mode_){
            rotation_angle = playback_scale_ * dt / PLAYBACK_WINDOW * 360 /500;
            global_model_matrix = global_model_matrix*Matrix4::Rotation(Point3(0,0,0), Vector3(0,1,0), rotation_angle);
        }
    }



