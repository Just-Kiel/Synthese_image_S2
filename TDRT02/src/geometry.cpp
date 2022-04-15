#include "geometry.h"
#include <math.h>
#include <stdio.h>

// Construit le point (x,y,z)
Point3D createPoint(float x, float y, float z){
    Point3D point;
    point.x = x;
    point.y = y;
    point.z = z;
    return point;
};

// Construit le vecteur (x, y, z)
Vector3D createVector(float x, float y, float z){
    Vector3D vector;
    vector.x = x;
    vector.y = y;
    vector.z = z;
    return vector;
};

// Construit le vecteur reliant les points P1 et P2
Vector3D createVectorFromPoints(Point3D p1, Point3D p2){
    return createVector(p2.x-p1.x, p2.y-p1.y, p2.z-p1.z);
};

// Construit le point P + V (i.e. translation de P par V)
Point3D pointPlusVector(Point3D p, Vector3D v){
    return createPoint(p.x + v.x, p.y + v.y, p.z + v.z);
};

// Addition et soustraction des vecteurs V1 et V2
Vector3D addVectors(Vector3D v1, Vector3D v2){
    return createVector(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
};

Vector3D subVectors(Vector3D v1, Vector3D v2){
    return createVector(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
};

// Multiplication et division d'un vecteur V par un scalaire a
Vector3D multVector(Vector3D v, float a){
    return createVector(v.x*a, v.y*a, v.z*a);
};

Vector3D divVector(Vector3D v, float a){
    return createVector(v.x/a, v.y/a, v.z/a);
};

// Produit scalaire des vecteurs V1 et V2
// détermine si ils sont orthogonaux
float dot(Vector3D a, Vector3D b){
    float dot;
    dot = (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
    return dot;
};

// Norme d'un vecteur V
float norm(Vector3D v){
    float norm;

    norm = (float)sqrt( v.x*v.x + v.y*v.y+ v.z*v.z );

    return norm;
};

// Construit le vecteur normalisé du vecteur V
Vector3D normalize(Vector3D v){
    float divisor = norm(v);
    return divVector(v, divisor);
};

void printPoint3D(Point3D p){
    printf("(%f, %f, %f)", p.x, p.y, p.z);
};

void printVector3D(Vector3D v){
    printf("(%f, %f, %f)", v.x, v.y, v.z);
};