#include <stdlib.h>
#include <stdio.h>

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
    //Scene scene = createScene();

    // Ajout d'objets dans la scene
    // TODO

    // Appel de la fonction de raytracing

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
