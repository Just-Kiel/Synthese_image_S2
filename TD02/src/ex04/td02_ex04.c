#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Question 6 : en openGL il vaut mieux translater avant de rotater pour permettre de tourner et se placer en local

/* Dimensions initiales et titre de la fenetre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;
static const char WINDOW_TITLE[] = "TD02 Exo 04";

/* Espace fenetre virtuelle */
static const float GL_VIEW_SIZE = 6.;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;


typedef struct {
    float x;
    float y;
}Vertex;

static float aspectRatio;

void onWindowResized(unsigned int width, unsigned int height)
{ 
    aspectRatio = width / (float) height;

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

    WINDOW_WIDTH=width;
    WINDOW_HEIGHT=height;
}

void drawOrigin(){
    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex2f(0.,0.);
        glVertex2f(1.,0.);

        glColor3f(0, 1, 0);
        glVertex2f(0.,0.);
        glVertex2f(0.,1.);
    glEnd();
}

void drawSquare(float red, float green, float blue, float posX, float posY){
    glColor3f(red, green, blue);
    
    glBegin(GL_LINE_LOOP);
        glVertex2f(posX-0.5, posY+0.5);
        glVertex2f(posX+0.5, posY+0.5);
        glVertex2f(posX+0.5, posY-0.5);
        glVertex2f(posX-0.5, posY-0.5);
    glEnd();
    glLoadIdentity();

}

void drawCircle(float red, float green, float blue, float posX, float posY){
    glColor3f(red, green, blue);
    glBegin(GL_LINE_LOOP);
        for(int i =0; i<51; i++){
            glVertex2f((sin(2*M_PI*i/50)*0.5)+posX, (cos(2*M_PI*i/50)*0.5)+posY);
        }
        
    glEnd();
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

  
    Vertex table[10];
    GLuint primitive = GL_POINTS;

    float red_circle = 255./255.;
    float green_circle = 69./255.;
    float blue_circle = 0./255.;

    float clickPosX = 0.;
    float clickPosY = 0.;

    int count = 0;
    /* Boucle principale */
    int loop = 1;
    while(loop) 
    {
        /* Recuperation du temps au debut de la boucle */
        Uint32 startTime = SDL_GetTicks();
        
        /* Placer ici le code de dessin */
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        drawOrigin();

        glRotatef(45, 0., 0., 1.);
        glTranslatef(2.4, 0.,0.);
        drawSquare(1., 0., 0., 0., 0.);
        
        // la translation permet ensuite de rotater sur l'axe central
        glTranslatef(2.4, 0.,0.);
        glRotatef(45, 0., 0., 1.);
        drawSquare(1., 0., 1., 0., 0.);

        
        drawCircle(red_circle, green_circle, blue_circle, 1., 2.);

        drawSquare(1., 1., 0., clickPosX, clickPosY);
        
        glBegin(primitive);
        glColor3f(1,1,1);
            for(int i=0; i<count; i++){
                glVertex2f(table[i].x, table[i].y);
            }
        glEnd();

        
        
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
                    if(count<10){
                        table[count].x=(-1+2.*e.button.x/(float)WINDOW_WIDTH)*(GL_VIEW_SIZE/2);
                        table[count].y=-(-1+2.*e.button.y/(float)WINDOW_HEIGHT)*(GL_VIEW_SIZE/2);
                        count++;
                    }
                    clickPosX = (-1+2.*e.button.x/(float)WINDOW_WIDTH)*(GL_VIEW_SIZE/2)*aspectRatio;
                    clickPosY = -(-1+2.*e.button.y/(float)WINDOW_HEIGHT)*(GL_VIEW_SIZE/2);
                    break;
                
                /* Touche clavier */
                case SDL_KEYDOWN:
                    printf("touche pressee (code = %d)\n", e.key.keysym.sym);
                    if(e.key.keysym.sym ==SDLK_a){
                        primitive=GL_LINE_LOOP;
                    }
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
