#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "geometry.h"
#include "colors.h"
#include "shape.h"

typedef struct Ray
{
    Point3D origine;
    Vector3D direction;
} Ray;

typedef struct Intersection
{
    Point3D position;
    ColorRGB couleur;
} Intersection;

typedef struct Scene
{
    Sphere mesSpheres[5];
    int nbSphere;
} Scene;


Ray createRay(Point3D origine, Vector3D direction);

int intersectsSphere(Ray r, Sphere s, Intersection* intersection, float* t);

Scene createScene();

void addSphereToScene(Scene* scene, Sphere s);

int throwRayThroughScene(Ray r, Scene scene, Intersection* intersectionNearest);

void simpleRaytracing(const Scene* scene, SDL_Surface* framebuffer);

#endif