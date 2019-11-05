CSci-4611 Assignment 4: So You Think Ants Can Dance

Yingxin Wei

1. Hierarchical transforms

I use the function DrawBoneRecursive(const std::string &bone_name, const Matrix4 &parent_transform, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) to build the hierarchical transforms. The hierarchical transformation matrices are defined according to the skeleton of the ant. I use the following for loop to recursively determine the proper child_root_transform for each bone.

    Matrix4 child_root_transform = ctm * skeleton_.BoneSpaceToRotAxesSpace(bone_name) * pose_.JointRotation(bone_name) * skeleton_.RotAxesSpaceToBoneSpace(bone_name) * skeleton_.BoneSpaceToChildrenSpace(bone_name);

    for (int i=0; i<skeleton_.num_children(bone_name); i++) {
        DrawBoneRecursive(skeleton_.child_bone(bone_name, i), child_root_transform, view_matrix, proj_matrix);
    }


2. Draw an ant

In addition, I use QuickShapes to draw some simple geometries like spheres, cubes, cylinders, cones to represent the body of the ants. For example, I use some spheres to represent the femur and I use some different colors and shapes to draw the head.


    if (bone_name == "lfemur" || bone_name == "rfemur") {
        Matrix4 upper_femur_scale = Matrix4::Scale(Vector3(0.1, 0.13, 0.1));
        Matrix4 upper_femur_rotate = Matrix4::RotationZ(GfxMath::ToRadians(40.0));
        quick_shapes_.DrawSphere(ctm * upper_femur_scale * upper_femur_rotate, view_matrix, proj_matrix, Color(0,0,1));
    }

    if (bone_name == "head") {
        Matrix4 head = Matrix4::Scale(Vector3(0.1, 0.1, 0.1));
        quick_shapes_.DrawCylinder(ctm * head, view_matrix, proj_matrix, Color(100,0,0));
        
        Matrix4 nose_scale = Matrix4::Scale(Vector3(0.05, 0.05, 0.1));
        Matrix4 nose_rotate = Matrix4::RotationX(GfxMath::ToRadians(90.0));
        Matrix4 nose_trans = Matrix4::Translation(Vector3(0, 0.8, -0.3));
        quick_shapes_.DrawCone(ctm * nose_scale * nose_rotate * nose_trans, view_matrix, proj_matrix, Color(0, 0, 1));
    }


3. Special motions

For the special motions, I load the motion clips from amc fles, trim and uninteresting frames from the front and back of the clips. These motions are binded with the corresponding buttons, so we can play them by pressing “Motion X” buttons in the GUI. For the other 4 special motions, I use the same dance moves. They are 05_10.amc, 05_09.amc, 05_20.amc, and 05_06.amc.

    void DanceApp::OnMotion1BtnPressed() {
        ballet_ant_.OverlayClip(ballet_special1_, 100);
    }

    void DanceApp::OnMotion2BtnPressed() {
        // TODO: add a call similar to this:
         ballet_ant_.OverlayClip(ballet_special2_, 100);
    }

    void DanceApp::OnMotion3BtnPressed() {
        // TODO: add a call similar to this:
         ballet_ant_.OverlayClip(ballet_special3_, 100);
    }

    void DanceApp::OnMotion4BtnPressed() {
        // TODO: add a call similar to this:
         ballet_ant_.OverlayClip(ballet_special4_, 100);
    }

    void DanceApp::OnMotion5BtnPressed() {
        // TODO: add a call similar to this:
         ballet_ant_.OverlayClip(ballet_special5_, 100);
    }

    ballet_special2_.LoadFromAMC(Platform::FindFile("05_10.amc", searchPath_), *ballet_ant_.skeleton_ptr());
    ballet_special2_.TrimFront(288);
    ballet_special2_.TrimBack(200);
    ballet_special2_.CalcRelativeTranslations();

    ballet_special3_.LoadFromAMC(Platform::FindFile("05_09.amc", searchPath_), *ballet_ant_.skeleton_ptr());
    ballet_special3_.TrimFront(288);
    ballet_special3_.TrimBack(200);
    ballet_special3_.CalcRelativeTranslations();

    ballet_special4_.LoadFromAMC(Platform::FindFile("05_20.amc", searchPath_), *ballet_ant_.skeleton_ptr());
    ballet_special4_.TrimFront(288);
    ballet_special4_.TrimBack(200);
    ballet_special4_.CalcRelativeTranslations();

    ballet_special5_.LoadFromAMC(Platform::FindFile("05_06.amc", searchPath_), *ballet_ant_.skeleton_ptr());
    ballet_special5_.TrimFront(288);
    ballet_special5_.TrimBack(200);
    ballet_special5_.CalcRelativeTranslations();

