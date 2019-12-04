CSci 4611 Assignment 6 README
Yingxin Wei

1. Drawing in the sky:

Part 1: Projects the mouse position in normalized device coordinates to a 3D point on the "sky", which is really a huge sphere (radius = 1500) that the viewer is inside. This function should always return true since any screen point can successfully be projected onto the sphere. `sky_point` is set to the resulting 3D point. 

    bool Sky::ScreenPtHitsSky(const Matrix4 &view_matrix, const Matrix4 &proj_matrix,
                            const Point2 &normalized_screen_pt, Point3 *sky_point)
    {
        
        Matrix4 camera_matrix = view_matrix.Inverse();
        Point3 eye = camera_matrix.ColumnToPoint3(3);
        
        Point3 mouseIn3d = GfxMath::ScreenToNearPlane(view_matrix, proj_matrix, normalized_screen_pt);
        Ray eyeThroughMouse = Ray(eye, (mouseIn3d - eye).ToUnit());
        float t;
        return eyeThroughMouse.IntersectSphere(Point3::Origin(), 1500.0, &t, sky_point);
        
    }

Part 2: Creates a new sky stroke mesh by projecting each vertex of the 2D mesh onto the sky dome and saving the result as a new 3D mesh.

    void Sky::AddSkyStroke(const Matrix4 &view_matrix, const Matrix4 &proj_matrix,
                           const Mesh &stroke2d_mesh, const Color &stroke_color)
    {

        Mesh stroke3d_mesh = stroke2d_mesh;
        std::vector<Point3> sky_points;
        
        for (int i = 0; i < stroke2d_mesh.num_vertices(); i++) {
            Point3 point3d;
            Point2 point2d = Point2(stroke2d_mesh.vertex(i)[0], stroke2d_mesh.vertex(i)[1]);
            ScreenPtHitsSky(view_matrix, proj_matrix, point2d, &point3d);
            sky_points.push_back(point3d);
        }
        
        stroke3d_mesh.SetVertices(sky_points);
        
        SkyStroke sky_stroke_;
        sky_stroke_.mesh = stroke3d_mesh;
        sky_stroke_.color = stroke_color;
        strokes_.push_back(sky_stroke_);

    }

2. Editing the ground:

There are 3 major steps to the algorithm, outlined here:

Step 1. Define a plane to project the stroke onto.  The first and last points of the stroke are projected onto the ground plane.  The plane passes through these two points on the ground.  The plane has a normal vector that points toward the camera and is parallel to the ground plane. In order to get the normal vector of correct direction, the plane vector is determined according to the relationship of start and end points of the stroke.

    Point2 Start = stroke2d[0];
    Point3 Start_3D = Point3(0,0,0);
    ScreenPtHitsGround(view_matrix, proj_matrix, Start, &Start_3D);

    Point2 End = stroke2d[stroke2d.size() - 1];
    Point3 End_3D = Point3(0,0,0);
    ScreenPtHitsGround(view_matrix, proj_matrix, End, &End_3D);

    Vector3 plane_vector;
    if (End.x() > Start.x()){
        plane_vector = End_3D - Start_3D;
    }
    else{
        plane_vector = Start_3D - End_3D;
    }
    Vector3 up = Vector3(0,1,0);
    Vector3 plane_norm = plane_vector.Cross(up).ToUnit();



Step 2. Project the 2D stroke into 3D so that it lies on the "projection plane" defined in step 1.

    std::vector<Point3> stroke3d;
    for (int i = 0; i < stroke2d.size(); i++) {
        Point3 mouseIn3d = GfxMath::ScreenToNearPlane(view_matrix, proj_matrix, stroke2d[i]);
        Ray ray = Ray(eye, (mouseIn3d - eye).ToUnit());
        float i_time;
        Point3 point3d;
        ray.IntersectPlane(Start_3D, plane_norm, &i_time, &point3d);
        stroke3d.push_back(point3d);
    }


Step 3. Loop through all of the vertices of the ground mesh, and adjust the height of each based on the equations in section 4.5 of the paper.

    std::vector<Point3> new_verts;
    for (int i=0; i<ground_mesh_.num_vertices(); i++) {
        
        Point3 P = ground_mesh_.vertex(i); // original vertex
        
        // adjust P according to equations...

        Point3 P_in_plane = P.ClosestPointOnPlane(Start_3D, plane_norm);
        float d = P.DistanceToPlane(Start_3D, plane_norm);
        float h = hfunc(plane_norm, stroke3d, P_in_plane);
        float P_y;
        if (h != 0){
            float w_d = 1-pow((d/5),2);
            if (w_d < 0){
                w_d = 0;
            }
            P_y = (1-w_d) * P.y() + w_d * h;
        }
        else{
            P_y = P.y();
        }
        P = Point3(P.x(), P_y, P.z());
        new_verts.push_back(P);
    }





