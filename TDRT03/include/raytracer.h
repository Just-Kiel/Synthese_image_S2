#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <SDL/SDL.h>

#include "shape.h"

#define MAX_SPHERE_COUNT 10
#define MAX_LIGHT_COUNT 10


typedef struct Ray {
    Point3D origin;
    Vector3D direction;
} Ray;

typedef struct Intersection {
    Point3D position;
    Material material;
    Vector3D normal;
} Intersection;

typedef struct Light {
    Point3D position;
    ColorRGB color;
} Light;

typedef struct Scene {
    Sphere spheres[MAX_SPHERE_COUNT];
    int sphereCount;
    Light lights[MAX_LIGHT_COUNT];
    int lightCount;
} Scene;


Ray createRay(Point3D origin, Vector3D direction);
int intersectsSphere(Ray r, Sphere s, Intersection* i, float* t);

Light createLight(Point3D position, ColorRGB color);

Scene createScene();
void addSphereToScene(Scene* scene, Sphere s);
void addLightToScene(Scene* scene, Light l);
int throwRayThroughScene(Ray r, Scene scene, Intersection* intersectionNearest);

void simpleRaytracing(Scene scene, SDL_Surface* framebuffer);
void lambertRaytracing(Scene scene, SDL_Surface* framebuffer);
void phongRaytracing(Scene scene, SDL_Surface* framebuffer);

#endif