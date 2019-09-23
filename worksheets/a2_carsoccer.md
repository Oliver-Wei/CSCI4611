# Assignment 2 (Car Soccer) Worksheet

## Definitions

Use the following pseudocode definitions for Q1 and Q2:

```
struct Vector3 {
    float x,
    float y,
    float z,
};
struct Sphere {
    Vector3 position,
    float radius,
};
```

## Q1: Eulerian Integration

In computer graphics and animation, there are many forms of integration that
are used. For simple physics models like we have in Car Soccer, Eulerian
Integration is good enough. Eulerian Integration uses velocity and position
information from the current frame, and the elapsed time to produce a position
for the next frame. Write pseudocode for determining the position of the sphere in the
next frame:

*Hint: think back to the motion equations from introductory physics. Or, look
around in the assignment handout.*

```
Vector3 velocity = Vector3(1.0, 1.0, 1.0);
float dt = 20; // milliseconds

Sphere s = Sphere {
    position: Vector3(0.0, 0.0, 0.0),
    radius: 5.0,
};

s.position = /* --- Fill in the next frame position computation here --- */
```

Then, plug the constants (`velocity` and `dt`) into the next frame position computation, and give the resultant (numeric) position as a Vector3:

```
/* --- Fill in numeric Vector3 here (e.g. Vector3(0.0, 0.0, 0.0)) --- */
```


## Q2: Sphere Intersection

In this assignment, you will need to test intersections between spheres and
other objects. Using the information contained within each sphere struct,
write pseudocode to determine whether or not two spheres are intersecting
(which you can use for car/ball intersections):

```
bool sphereIntersection(Sphere s1, Sphere s2) {
    /* --- Fill in your sphere intersection code here --- */
}
```

To check that your intersections work, use your intersection math to fill out
if these test spheres will intersect:

```
Sphere s1 = Sphere {
    position: Vector3(0.0, 1.0, 0.0),
    radius: 1.0,
};

Sphere s2 = Sphere {
    position: Vector3(3.0, 0.0, 0.0),
    radius: 1.0,
};

Sphere s3 = Sphere {
    position: Vector3(1.0, 1.0, 0.0),
    radius: 2.0,
};

print(sphereIntersection(s1, s2));
/* --- Fill in expected output (True or False) --- */

print(sphereIntersection(s1, s3));
/* --- Fill in expected output (True or False) --- */
```
