#include <SDL2/SDL.h>
#include <img/img.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <random>




/* Dimensions initiales et titre de la fenetre */
static const unsigned int WINDOW_WIDTH = 800;
static const unsigned int WINDOW_HEIGHT = 600;
static const char WINDOW_TITLE[] = "TD05 Exercice 2";

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

typedef struct planet{
    int periode;
    float aphelie;
    float perihelie;
    int idTexture;
    int taille;
}planet;

float planetScale = 0.03f;
float OrbitScale = 3;
float rotateScale = 1;
int currentPlanet = 0;
float currentZoom = 1;
int mouseX = 0;
int mouseY = 0;

int moveX = 0;
int moveY = 0;
int drag = 0;

planet soleil = {1, 0, 0, 8, 20};
planet mercure = {87, 0.46f, 0.30f, 0, 4};
planet venus = {224, 0.72f, 0.71f, 2, 12};
planet terre = {365, 0.99f, 1.01f, 3, 12};
planet mars = {686, 1.66f, 1.38f, 4, 6};
planet jupiter = {4332, 5.45f, 4.95f, 1, 142};
planet saturne = {10759, 10.12f, 9.04f, 5, 120};
planet uranus = {30688, 20.09f, 18.28f, 6, 51};
planet neptune = {60195, 30.33f, 29.81f, 7, 49};

planet systemeSolaire[9]={soleil, mercure, venus, terre, mars, jupiter, saturne, uranus, neptune};

static const int CIRCLE_SUBDIVS = 2<<5;

void moveCamera(int idPlanet, planet* system, Uint32 temps){
    glScalef(currentZoom, currentZoom, currentZoom);
    glTranslatef(-cos(temps*rotateScale*2*M_PI/system[idPlanet].periode)*system[idPlanet].aphelie*OrbitScale + moveX*planetScale,
    -sin(temps*rotateScale*2*M_PI/system[idPlanet].periode)*system[idPlanet].aphelie*OrbitScale - moveY*planetScale, 0);
}

void drawCircle(int filled, float red, float green, float blue) 
{
    int i = 0;

    if(filled) 
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(red, green, blue);
        glVertex2f(0.0, 0.0);
    }
    else 
    {
        glBegin(GL_LINE_STRIP);
    }

    for(i = 0; i <= CIRCLE_SUBDIVS; i++) 
    {
        float x = cos(i * (2 * M_PI / (float) CIRCLE_SUBDIVS));
        float y = sin(i * (2 * M_PI / (float) CIRCLE_SUBDIVS));
        glTexCoord2f((x+1)/2, (-y+1)/2);
        glVertex2f(x, y);
    }
    // float newX = cos(i * (2 * M_PI / (float) CIRCLE_SUBDIVS)*10);
    // float newY = sin(i * (2 * M_PI / (float) CIRCLE_SUBDIVS)*10);
    // glTexCoord2f((newX+1)/2, (-newY+1)/2);
    // glVertex2f( newX, newY);

    glEnd();

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
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> distr(1.8f, 3.8f);

    float distance_asteroids[20] = {distr(eng), distr(eng), distr(eng), distr(eng), distr(eng), distr(eng), distr(eng), distr(eng), distr(eng), distr(eng), distr(eng), distr(eng), distr(eng), distr(eng), distr(eng), distr(eng), distr(eng), distr(eng), distr(eng), distr(eng)}; 
    
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

    GLuint idTex[9];
    {
        img::Image texture[9] = {img::load("images/mercury.jpg"), img::load("images/jupiter.jpg"), img::load("images/venus.jpg"), img::load("images/earth.jpg"), img::load("images/mars.jpg"), img::load("images/saturn.jpg"), img::load("images/uranus.jpg"), img::load("images/neptune.jpg"), img::load("images/sun.jpg")};

        
        glGenTextures(9, idTex);

        for(int i=0; i<9; i++){
            glBindTexture(GL_TEXTURE_2D, idTex[i]);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glTexImage2D(
                GL_TEXTURE_2D, 0, GL_RGBA, texture[i].width(), texture[i].height(),
                0, GL_RGBA, GL_UNSIGNED_BYTE, texture[i].data()
            );
        }
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    

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

        moveCamera(currentPlanet, systemeSolaire, startTime);

        // diametre du Soleil : 0.01UA = 1 10(-2) = 1

        // distance entre Mercure et le Soleil : 0.38UA
        // diametre de Mercure : 3.26 10(-5)UA = 0.00326  === 0.14 open gl --- rester en km
        glEnable(GL_TEXTURE_2D);

        for(int i=0; i<9; i++){
            glBindTexture(GL_TEXTURE_2D, idTex[systemeSolaire[i].idTexture]);

            glPushMatrix();
                glRotatef(startTime*rotateScale*360/systemeSolaire[i].periode, 0, 0, 1);
                // glTranslatef(OrbitScale*systemeSolaire[i].aphelie, 0, 0);
                glTranslatef(cos(OrbitScale*systemeSolaire[i].aphelie)+OrbitScale*systemeSolaire[i].aphelie,
                sin(OrbitScale*systemeSolaire[i].perihelie)+OrbitScale*systemeSolaire[i].perihelie, 0);
                glScalef(planetScale*systemeSolaire[i].taille, planetScale*systemeSolaire[i].taille, 0);
                drawCircle(1, 255.f/255.f, 255.f/255.f, 255.f/255.f);
            glPopMatrix();

            glBindTexture(GL_TEXTURE_2D, 0);
        }

        for(int i=0; i<20; i++){
            glPushMatrix();
                glRotatef(startTime*rotateScale*360/36550*(i+1), 0, 0, 1);
                glTranslatef(OrbitScale*distance_asteroids[i], 0, 0);
                glScalef(0.1f, 0.1f, 0.1f);
                drawCircle(1, 255.f/255.f, 255.f/255.f, 255.f/255.f);
            glPopMatrix();
        }

        // distance entre Venus et le Soleil : 0.72UA
        // diametre de Venus : 8.09 10(-5)UA

        // distance entre Terre et le Soleil : 1UA
        // diametre de Terre : 8.53 10(-5)UA

        // distance entre Mars et le Soleil : 1.52UA
        // diametre de Mars : 4.54 10(-5)UA
        // glBindTexture(GL_TEXTURE_2D, idTex[4]);
        // glPushMatrix();
        //     glRotatef(startTime*rotateScale*360/686, 0, 0, 1);
        //     glTranslatef(OrbitScale*1.52f, 0, 0);
        //     glScalef(planetScale*6, planetScale*6, 0);
        //     drawCircle(1, 255.f/255.f, 255.f/255.f, 255.f/255.f);
        // glPopMatrix();
        // glBindTexture(GL_TEXTURE_2D, 0);

        // distance entre Jupiter et le Soleil : 5.20UA
        // diametre de Jupiter : 0.001UA === 5 ou 6 open gl
        // glEnable(GL_TEXTURE_2D);
        // glBindTexture(GL_TEXTURE_2D, idTex[1]);
        // glPushMatrix();
        //     glRotatef(startTime*rotateScale*360/4332, 0, 0, 1);
        //     glTranslatef(OrbitScale*5.20f, 0, 0);
        //     glScalef(planetScale*142, planetScale*142, 0);
        //     drawCircle(1, 255.f/255.f, 255.f/255.f, 255.f/255.f);
        // glPopMatrix();
        // glBindTexture(GL_TEXTURE_2D, 0);
        

        // distance entre Saturne et le Soleil : 9.55UA
        // diametre de Saturne : 0.001UA
        // glBindTexture(GL_TEXTURE_2D, idTex[5]);
        // glPushMatrix();
        //     glRotatef(startTime*rotateScale*360/10759, 0, 0, 1);
        //     glTranslatef(OrbitScale*9.55, 0, 0);
        //     glScalef(planetScale*120, planetScale*120, 0);
        //     drawCircle(1, 210.f/255.f, 180.f/255.f, 140.f/255.f);
        // glPopMatrix();
        // glBindTexture(GL_TEXTURE_2D, 0);

        // distance entre Uranus et le Soleil : 19.19UA
        // diametre de Uranus : 0.00034UA
        // glBindTexture(GL_TEXTURE_2D, idTex[6]);
        // glPushMatrix();
        //     glRotatef(startTime*rotateScale*360/30688, 0, 0, 1);
        //     glTranslatef(OrbitScale*19.19, 0, 0);
        //     glScalef(planetScale*51, planetScale*51, 0);
        //     drawCircle(1, 173.f/255.f, 216.f/255.f, 230.f/255.f);
        // glPopMatrix();
        // glBindTexture(GL_TEXTURE_2D, 0);

        // distance entre Neptune et le Soleil : 30.11UA = 75/30  1UA = 2 openGL
        // diametre de Neptune : 0.00033UA
        // glBindTexture(GL_TEXTURE_2D, idTex[7]);
        // glPushMatrix();
        //     glRotatef(startTime*rotateScale*360/60195, 0, 0, 1);
        //     glTranslatef(OrbitScale*30, 0, 0);
        //     glScalef(planetScale*49, planetScale*49, 0);
        //     drawCircle(1, 0.f/255.f, 0.f/255.f, 205.f/255.f);
        // glPopMatrix();
        // glBindTexture(GL_TEXTURE_2D, 0);
            

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
                    drag = 0;
                    // mouseX = (WINDOW_WIDTH/2) - e.button.x;
                    // mouseY = (WINDOW_HEIGHT/2) - e.button.y;
                    // printf("%d oupsi %d\n", WINDOW_WIDTH, e.button.x);
                    // printf("mouseX %d\n", mouseX);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    drag = 1;
                    mouseX=e.button.x;
                    mouseY = e.button.y;
                    break;

                case SDL_MOUSEWHEEL:
                    currentZoom=currentZoom+e.wheel.y;
                    break;
                case SDL_MOUSEMOTION :
                    if(drag){
                        moveX = e.motion.x - mouseX;
                        moveY = e.motion.y - mouseY;
                    } else {
                        moveX = 0;
                        moveY = 0;
                    }
                    break;
                    
                
                /* Touche clavier */
                case SDL_KEYDOWN:
                    printf("touche pressee (code = %d)\n", e.key.keysym.sym);
                    switch (e.key.keysym.sym)
                    {
                    case SDLK_0:
                        currentPlanet = 0;
                        break;
                    case SDLK_1:
                        currentPlanet = 1;
                        break;
                    case SDLK_2:
                        currentPlanet = 2;
                        break;
                    case SDLK_3:
                        currentPlanet = 3;
                        break;
                    case SDLK_4:
                        currentPlanet = 4;
                        break;                    
                    case SDLK_5:
                        currentPlanet = 5;
                        break;
                    case SDLK_6:
                        currentPlanet = 6;
                        break;
                    case SDLK_7:
                        currentPlanet = 7;
                        break;
                    case SDLK_8:
                        currentPlanet = 8;
                        break;
                    default:
                        break;
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