#ifndef SHAPE_H
#define SHAPE_H

#include "geometry.h"
#include "colors.h"

typedef struct Sphere
{
    Point3D centre;
    float rayon;
    ColorRGB couleur;
} Sphere ;

Sphere createSphere(Point3D centre, float rayon, ColorRGB couleur);

#endif