CSci 4611 Assignment 6 README
Yingxin Wei

1. Drawing in the sky:

As mentioned in Requirement 1, I create a mesh called m, to store the points of stroke on the screen. Since it guanranteed that the start point is on the sky, I directly loop all the 2d points that in the stroke to find their 3d positions on teh sky by using function ScreenPtHitsSky():

    for (int i = 0; i < vertex_length; i++) {
        Point3 final_point = Point3(0,0,0);
        Point2 draw_vertex = Point2(m.Mesh::vertex(i)[0], m.Mesh::vertex(i)[1]);
        ScreenPtHitsSky(view_matrix, proj_matrix, draw_vertex, &final_point);
        sky_points.push_back(final_point);
        
    }

2. Editing the ground:

To define a plane, I need to find 2 points and a vector as normal. Therefore, I firstly use ScreenPtHitsGround to find the 3d position of the starting and ending points of strokes:

    Point2 Start = stroke2d[0];
    Point3 Start_3D = Point3(0,0,0);
    ScreenPtHitsGround(view_matrix, proj_matrix, Start, &Start_3D);

    Point2 End = stroke2d[stroke2d.size() - 1];
    Point3 End_3D = Point3(0,0,0);
    ScreenPtHitsGround(view_matrix, proj_matrix, End, &End_3D);

At the same time, the normal of the plane is calculated by the up vector:(0,1,0) and vector in the plane: End_3d - Start_3d:

    Vector3 up = Vector3(0,1,0);
    Vector3 plane_vector = End_3D - Start_3D;
    Vector3 plane_Norm = (Vector3::Cross(plane_vector, up)).ToUnit();

To project the point on the plane, I loop all the 2d points in strokes and use ray to find their 3d points. IntersectPlane() is used here:

    std::vector<Point3> stroke_3d;
    int point_num = stroke2d.size();
    for (int i = 0; i < point_num; i++) {
        Point3 pt3d = GfxMath::ScreenToNearPlane(view_matrix, proj_matrix, stroke2d[i]);
        Ray ray(eye, (pt3d - eye).ToUnit());
        float i_time;
        //        int i_tri;
        Point3 plane_point = Point3(0,0,0);
        ray.IntersectPlane(Start_3D, plane_Norm, &i_time, &plane_point);
        std::cout << plane_point << std::endl;
        stroke_3d.push_back(plane_point);
    }

To find the closest point of a mesh point on the groud to the plane, I use ray.IntersectPlane() to find it. In order to make a complete mountain based on drawing, I use if statement to determine the direction of ray:

    if (ray.IntersectPlane(Start_3D, plane_Norm, &i_time, &closest_point) == true) {
        ray.IntersectPlane(Start_3D, plane_Norm, &i_time, &closest_point);
    }
    else if (ray.IntersectPlane(Start_3D, -plane_Norm, &i_time, &closest_point) == true) {
        ray.IntersectPlane(Start_3D, -plane_Norm, &i_time, &closest_point);
    }

This guaranteed that the points in the front of the plane and behind the plane could be changed as a mountain.

Finally, the y parameters are calculated based on the equations:

    float height = hfunc(plane_Norm, stroke_3d, closest_point);

    float P_y;
    if (height != 0) {
        float weight = 1 - pow((distance / 5.0),2.0);
        if (weight < 0) {
            weight = 0;
        }

        P_y = (1-weight) * P[1] + weight * height;
    }
    else {
        P_y = P[1];
    }

    P = Point3(P[0], P_y, P[2]);
    new_verts.push_back(P);





