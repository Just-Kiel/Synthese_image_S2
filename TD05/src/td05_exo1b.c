#include <SDL2/SDL.h>
#include <img/img.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

/* Récupération de l'heure système */
time_t rawtime;
struct tm* timeinfo;

/* Dimensions initiales et titre de la fenetre */
static const unsigned int WINDOW_WIDTH = 800;
static const unsigned int WINDOW_HEIGHT = 600;
static const char WINDOW_TITLE[] = "TD05 Exercice 1b";

/* Espace fenetre virtuelle */
static const float GL_VIEW_SIZE = 150.;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;


void onWindowResized(unsigned int width, unsigned int height)
{ 
    float aspectRatio = width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if( aspectRatio > 1) 
    {
        gluOrtho2D(
        -GL_VIEW_SIZE / 2. * aspectRatio, GL_VIEW_SIZE / 2. * aspectRatio, 
        -GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.);
    }
    else
    {
        gluOrtho2D(
        -GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.,
        -GL_VIEW_SIZE / 2. / aspectRatio, GL_VIEW_SIZE / 2. / aspectRatio);
    }
}

GLuint drawSquare(){
    GLuint id = glGenLists(1);

    glNewList(id, GL_COMPILE);
    glRotatef(180, 1, 0,0);
        glBegin(GL_QUADS);

        

        glTexCoord2f(1.0, 1.0);
        // glTexCoord2f(0.0, 0.0);
        glVertex2f( 0.5 , -0.5);

        // glTexCoord2f(1.0, 0.0);
        glTexCoord2f(1.0, 0.0);
        glVertex2f( 0.5 , 0.5);

        // glTexCoord2f(0.0, 1.0);
        glTexCoord2f(0.0, 0.0);
        glVertex2f( -0.5 , 0.5);

        // glTexCoord2f(1.0, 1.0);
        glTexCoord2f(0.0, 1.0);
        glVertex2f( -0.5 , -0.5);

        // glVertex2f( 0.5 , -0.5);

        glEnd();
    glEndList();

    return id;
}

int main(int argc, char** argv) 
{
    printf("hello");
    /* Initialisation de la SDL */

    if(SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        const char* error = SDL_GetError();
        fprintf(
            stderr, 
            "Erreur lors de l'intialisation de la SDL : %s\n", error);

        SDL_Quit();
        return EXIT_FAILURE;
    }
	
    /* Ouverture d'une fenetre et creation d'un contexte OpenGL */

    SDL_Window* window;
    {
        window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        if(NULL == window) 
        {
            const char* error = SDL_GetError();
            fprintf(
                stderr,
                "Erreur lors de la creation de la fenetre : %s\n", error);

            SDL_Quit();
            return EXIT_FAILURE;
        }
    }
    
    SDL_GLContext context;
    {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        context = SDL_GL_CreateContext(window);
    
        if(NULL == context) 
        {
            const char* error = SDL_GetError();
            fprintf(
                stderr,
                "Erreur lors de la creation du contexte OpenGL : %s\n", error);

            SDL_DestroyWindow(window);
            SDL_Quit();
            return EXIT_FAILURE;
        }
    }    
  
    onWindowResized(WINDOW_WIDTH, WINDOW_HEIGHT);

    GLuint idTex[11];
    {
        img::Image texture[11] = {img::load("images/numbers/0.png"), img::load("images/numbers/1.png"), img::load("images/numbers/2.png"), img::load("images/numbers/3.png"), img::load("images/numbers/4.png"), img::load("images/numbers/5.png"), img::load("images/numbers/6.png"), img::load("images/numbers/7.png"), img::load("images/numbers/8.png"), img::load("images/numbers/9.png"), img::load("images/numbers/colon.png")};

        
        glGenTextures(11, idTex);

        for(int i=0; i<11; i++){
            glBindTexture(GL_TEXTURE_2D, idTex[i]);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glTexImage2D(
                GL_TEXTURE_2D, 0, GL_RGBA, texture[i].width(), texture[i].height(),
                0, GL_RGBA, GL_UNSIGNED_BYTE, texture[i].data()
            );
        }
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Boucle principale */
    int loop = 1;
    while(loop) 
    {
        /* Récupération du temps */
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        /* Recuperation du temps au debut de la boucle */
        Uint32 startTime = SDL_GetTicks();
        
        /* Placer ici le code de dessin */
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Pour les secondes
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, idTex[timeinfo->tm_sec%10]);
        
        glScalef(15, 20, 0);

        glPushMatrix();
            glTranslatef(4, 0, 0);
            glCallList(drawSquare());
        glPopMatrix();
        
        glBindTexture(GL_TEXTURE_2D, idTex[timeinfo->tm_sec/10]);
        
        glPushMatrix();
            glTranslatef(2.8, 0, 0);
            glCallList(drawSquare());
        glPopMatrix();

        //les colons
        glBindTexture(GL_TEXTURE_2D, idTex[10]);

        glPushMatrix();
            glTranslatef(2, 0, 0);
            glCallList(drawSquare());
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(-0.8, 0, 0);
            glCallList(drawSquare());
        glPopMatrix();

        //les minutes
        glBindTexture(GL_TEXTURE_2D, idTex[timeinfo->tm_min%10]);

        glPushMatrix();
            glTranslatef(1, 0, 0);
            glCallList(drawSquare());
        glPopMatrix();
        
        glBindTexture(GL_TEXTURE_2D, idTex[timeinfo->tm_min/10]);
        
        glPushMatrix();
            glTranslatef(0, 0, 0);
            glCallList(drawSquare());
        glPopMatrix();

        //les heures
        glBindTexture(GL_TEXTURE_2D, idTex[timeinfo->tm_hour%10]);

        glPushMatrix();
            glTranslatef(-1.8, 0, 0);
            glCallList(drawSquare());
        glPopMatrix();
        
        glBindTexture(GL_TEXTURE_2D, idTex[timeinfo->tm_hour/10]);
        
        glPushMatrix();
            glTranslatef(-2.8, 0, 0);
            glCallList(drawSquare());
        glPopMatrix();



        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
        
        /* Echange du front et du back buffer : mise a jour de la fenetre */
        SDL_GL_SwapWindow(window);
        
        /* Boucle traitant les evenements */
        SDL_Event e;
        while(SDL_PollEvent(&e)) 
        {
            /* L'utilisateur ferme la fenetre : */
			if(e.type == SDL_QUIT) 
			{
				loop = 0;
				break;
			}
		
			if(	e.type == SDL_KEYDOWN 
				&& (e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_ESCAPE))
			{
				loop = 0; 
				break;
			}
            
            switch(e.type) 
            {
                case SDL_WINDOWEVENT:
                    switch (e.window.event) 
                    {
                        /* Redimensionnement fenetre */
                        case SDL_WINDOWEVENT_RESIZED:
                            onWindowResized(e.window.data1, e.window.data2);                
                            break;

                        default:
                            break; 
                    }
                    break;

                /* Clic souris */
                case SDL_MOUSEBUTTONUP:
                    printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                    // printf("hours %d\n", timeinfo->tm_hour);
                    // printf("minutes %d\n", timeinfo->tm_min);
                    // printf("seconds %d\n", timeinfo->tm_sec);
                    printf("seconds %d\n", timeinfo->tm_sec%10);

                    break;
                
                /* Touche clavier */
                case SDL_KEYDOWN:
                    printf("touche pressee (code = %d)\n", e.key.keysym.sym);
                    break;
                    
                default:
                    break;
            }
        }

        /* Calcul du temps ecoule */
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        /* Si trop peu de temps s'est ecoule, on met en pause le programme */
        if(elapsedTime < FRAMERATE_MILLISECONDS) 
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    /* Liberation des ressources associees a la SDL */ 
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}