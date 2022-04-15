#include "shape.h"

Sphere createSphere(Point3D centre, float rayon, ColorRGB couleur){
    Sphere mySphere;

    mySphere.centre = centre;
    mySphere.rayon = rayon;
    mySphere.couleur = couleur;

    return mySphere;
};