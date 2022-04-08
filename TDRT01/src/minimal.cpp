// #include <SDL2/SDL.h>
#if defined(_WIN32)
#include <windows.h>
#endif
// #include <GL/gl.h>
// #include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include "geometry.h"

int main(int argc, char** argv) 
{
    Point3D point0 = createPoint(0, 0, 0);
    Point3D point120 = createPoint(1, 2, 0);
    printf("pointPlusVector : ");
    printPoint3D(point0);
    printf(" + ");
    printPoint3D(point120);
    printf(" = ");
    printPoint3D(pointPlusVector(point0, point120));
    printf("\n");

    Point3D point0512 = createPoint(0.5, 1, -2);
    Point3D point210 = createPoint(0.2, -1, 0);
    printf("addVectors : ");
    printPoint3D(point0512);
    printf(" + ");
    printPoint3D(point210);
    printf(" = ");
    printPoint3D(addVectors(point0512, point210));
    printf("\n");

    printf("subVectors : ");
    printPoint3D(point0512);
    printf(" - ");
    printPoint3D(point210);
    printf(" = ");
    printPoint3D(subVectors(point0512, point210));
    printf("\n");

    printf("multVector : ");
    printPoint3D(point0512);
    printf("*2.0 = ");
    printPoint3D(multVector(point0512, 2.0));
    printf("\n");

    printf("multVector : ");
    printPoint3D(point0512);
    printf("*0.0 = ");
    printPoint3D(multVector(point0512, 0.0));
    printf("\n");

    printf("divVector : ");
    printPoint3D(point0512);
    printf("/2.0 = ");
    printPoint3D(divVector(point0512, 2.0));
    printf("\n");

    printf("divVector : ");
    printPoint3D(point0512);
    printf("/0.0 = ");
    printPoint3D(divVector(point0512, 0.0));
    printf("\n");

    //already verified

    Vector3D point1 = createVector(1, 0, 0);
    Vector3D point2 = createVector(2, 0, 0);
    printf("dot : ");
    printVector3D(point1);
    printf(".");
    printVector3D(point2);
    printf("= %f", dot(point1, point2));
    printf("\n");

    Vector3D vec01 = createVector(0, 1, 0);
    printf("dot : ");
    printVector3D(point1);
    printf(".");
    printVector3D(vec01);
    printf("= %f", dot(point1, vec01));
    printf("\n");

    printf("norm : ||");
    printVector3D(point2);
    printf("|| = %f", norm(point2));
    printf("\n");

    Vector3D vec111 = createVector(1, 1, 1);
    printf("norm : ||");
    printVector3D(vec111);
    printf("|| = %f", norm(vec111));
    printf("\n");

    printf("normalize : ");
    printVector3D(vec111);
    printf(" = ");
    printVector3D(normalize(vec111));
    printf("\n");
    
    Vector3D vec000 = createVector(0, 0, 0);
    printf("normalize : ");
    printVector3D(vec000);
    printf(" = ");
    printVector3D(normalize(vec000));
    printf("\n");

    return EXIT_SUCCESS;
}