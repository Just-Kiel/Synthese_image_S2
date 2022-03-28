#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
static const char WINDOW_TITLE[] = "TD05 Exercice 1a";

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

GLuint drawCircle(){
    GLuint id = glGenLists(1);

    glNewList(id, GL_COMPILE);
            glBegin(GL_LINE_LOOP);
                for(int i =0; i<51; i++){
                    glVertex2f(sin(2*M_PI*i/50)*0.5, cos(2*M_PI*i/50)*0.5);
                } 
            glEnd();
    glEndList();

    return id;
}

GLuint drawSquare(int filled){
    GLuint id = glGenLists(1);

    glNewList(id, GL_COMPILE);

        if(filled) 
        {
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.0, 0.0);
        }
        else 
        {
            glBegin(GL_LINE_STRIP);
        }
        //glBegin(GL_LINE_LOOP);
        //     glVertex2f(-0.5, 0.5);
        //     glVertex2f(0.5, 0.5);
        //     glVertex2f(0.5, -0.5);
        //     glVertex2f(-0.5, -0.5);
        // glEnd();

        glVertex2f( 0.5 , -0.5);
        glVertex2f( 0.5 , 0.5);
        glVertex2f( -0.5 , 0.5);
        glVertex2f( -0.5 , -0.5);
        glVertex2f( 0.5 , -0.5);

        glEnd();
    glEndList();

    return id;
}

int main(int argc, char** argv) 
{
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

        //tracé du cercle de l'horloge
        glPushMatrix();
            glScalef(120, 120, 0);
            glCallList(drawCircle());
        glPopMatrix();

        //tracé des ptits traits
        for(int i =0; i<60; i++){
            glPushMatrix(); 
                glRotatef(360/60*i, 0, 0, 1);
                glTranslatef(-55, 0, 0);
                
                glScalef(5, 2, 1);
                if(i%5==0){
                    glCallList(drawSquare(1));
                } else {
                    glCallList(drawSquare(0));
                }
            glPopMatrix();
        }


        //tracé des secondes
        glPushMatrix();
            glRotatef((-360/60*timeinfo->tm_sec)-90, 0, 0, 1);
        
            glTranslatef(-25, 0, 0);
            glScalef(50, 0.7, 1);

            glCallList(drawSquare(1));
        glPopMatrix();
        
        //tracé des minutes
        glPushMatrix();
            glRotatef((-360/60*timeinfo->tm_min)-90, 0, 0, 1);
        
            glTranslatef(-15, 0, 0);
            glScalef(30, 1, 1);

            glCallList(drawSquare(1));
        glPopMatrix();
        
        //tracé des heures
        glPushMatrix();
            glRotatef((-360/12*timeinfo->tm_hour)-90, 0, 0, 1);
        
            glTranslatef(-10, 0, 0);
            glScalef(20, 1.5, 1);

            glCallList(drawSquare(0));
        glPopMatrix();
        
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
                    printf("hours %d\n", timeinfo->tm_hour);
                    printf("minutes %d\n", timeinfo->tm_min);
                    printf("seconds %d\n", timeinfo->tm_sec);

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
