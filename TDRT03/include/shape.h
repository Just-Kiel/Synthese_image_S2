#ifndef SHAPE_H
#define SHAPE_H

#include "geometry.h"
#include "shading.h"

typedef struct Sphere {
    Point3D center;
    float radius;
    Material material;
} Sphere;

Sphere createSphere(Point3D center, float radius, Material material);

#endif