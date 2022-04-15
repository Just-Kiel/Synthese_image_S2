#include <SDL2/SDL.h>
#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
// #include "geometry.h"
#include "raytracer.h"

static const unsigned int WINDOW_WIDTH = 800;
static const unsigned int WINDOW_HEIGHT = 800;
static const unsigned int BIT_PER_PIXEL = 32;

int main(int argc, char** argv) 
{
    // Initialisation de la SDL
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    // Creation de la fenetre SDL
	SDL_Window* window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if(window == NULL) {
		fprintf(stderr, "Impossible de créer la fenêtre. Fin du programme.\n");
        return EXIT_FAILURE;
	}

    SDL_Surface* surface = SDL_GetWindowSurface(window);
    if(surface == NULL) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if(renderer == NULL) {
        fprintf(stderr, "Impossible de créer le renderer. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    // Creation d'une surface SDL dans laquelle le raytracer dessinera
    SDL_Surface* framebuffer = SDL_CreateRGBSurface(0, WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, 0, 0, 0, 0);
	if(framebuffer == NULL) {
        fprintf(stderr, "Erreur d'allocation pour le framebuffer. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    // Nettoyage du framebuffer par une couleur de fond (noir)
    SDL_FillRect(framebuffer, NULL, SDL_MapRGB(framebuffer->format, 0, 0, 0));

    // Creation de la scene
    Scene scene = createScene();

    // Ajout d'objets dans la scene
    // TODO
    Sphere test = createSphere(createPoint(0, 0, -3), 1, createColor(1, 0, 0));
    Sphere test2 = createSphere(createPoint(0.25, 0, -2), 0.5, createColor(0, 1, 0));
    Ray rayon = createRay(createPoint(0, 0, 0), normalize(createVector(0, 0, -1)));
    Intersection intersect;
    float t;

    addSphereToScene(&scene, test2);
    addSphereToScene(&scene, test);

    // Appel de la fonction de raytracing
    int result = intersectsSphere(rayon, test, &intersect, &t);
    printf("Result = %d et t = %f", result, t);

    result = throwRayThroughScene(rayon, scene, &intersect);
    printf("Result = %d et inter = %f, %f, %f et color = %f, %f, %f", result, intersect.position.x, intersect.position.y, intersect.position.z, intersect.couleur.r, intersect.couleur.g, intersect.couleur.b);

    simpleRaytracing(&scene, framebuffer);

	// Création de la texture pour l'affichage dans la fenêtre
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, framebuffer);
	SDL_FreeSurface(framebuffer);

    int loop = 1;
    while(loop) {

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                loop = 0;
                break;
            }
        }
    }

    SDL_Quit();

    return EXIT_SUCCESS;
}