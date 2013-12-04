// Demo of heavily simplified sprite engine
// by Ingemar Ragnemalm 2009
// used as base for lab 4 in TSBK03.
// OpenGL 3 conversion 2013.

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif

#include <stdlib.h>
#include "LoadTGA.h"
#include "SpriteLight.h"
#include "GL_utilities.h"
#include <math.h>

// Lgg till egna globaler hr efter behov.


TextureData *sheepFace, *blackFace, *dogFace, *foodFace;



float cohesion_constant = 0.0;
float separation_constant = 0.0; //3.0/300.0;
float align_constant = 0.0;
float separation_dist_sqr = 2000.0;
float group_dist_sqr = 20000.0; // squared distance for cohesion

const float max_speed = 2.0;

float avoidanceCalc(float input, float distance);

void SpriteBehavior() // Din kod!
{
// Lgg till din labbkod hr. Det gr bra att ndra var som helst i
// koden i vrigt, men mycket kan samlas hr. Du kan utg frn den
// globala listroten, gSpriteRoot, fr att kontrollera alla sprites
// hastigheter och positioner, eller arbeta frn egna globaler.
	SpritePtr sp, spC;
    int tot_sprites;
    float tot_h, tot_v, current_sheep_pos_v, current_sheep_pos_h, distance, group_h, group_v;
    float speed_h, speed_v, tot_speed_h, tot_speed_v, normal_speed;

    
    /*Cohesion+separation*/ 
    
    sp = gSpriteRoot;
   

	do // for each sheep
	{
        current_sheep_pos_v = sp->position.v;
        current_sheep_pos_h = sp->position.h;
        spC = gSpriteRoot;
        tot_h = 0.0;
        tot_v = 0.0; 
        group_h = 0.0; 
        group_v = 0.0;
        speed_h = 0.0;
        speed_v = 0.0;
        tot_speed_h = 0.0;
        tot_speed_v = 0.0;
        tot_sprites = 0.0;

        while (spC != NULL) {
            



            distance = pow(current_sheep_pos_v - spC->position.v,2.0) + 
                pow(current_sheep_pos_h - spC->position.h,2.0);


            if (distance < group_dist_sqr) {           
                tot_h += spC->position.h;
                tot_v += spC->position.v;
                
                tot_speed_h += spC->speed.h; 
                tot_speed_v += spC->speed.v;
                tot_sprites++; 
            }


            if ((distance < separation_dist_sqr) && (distance > 0) ) {//&& sp->face != blackFace) {           
                
                sp->speed.h += avoidanceCalc(sp->position.h - spC->position.h, distance)*separation_constant; 
                sp->speed.v += avoidanceCalc(sp->position.v - spC->position.v, distance)*separation_constant; 
            }
            //printf("sheep face is: %i \n", spC->face);
            spC = spC->next; 
        }
       

        if (tot_sprites > 0 && sp->face !=blackFace) {
            group_h = tot_h/tot_sprites;
            group_v = tot_v/tot_sprites;

            speed_h = tot_speed_h / tot_sprites;
            speed_v = tot_speed_v / tot_sprites;
             


            sp->speed.h += (group_h - current_sheep_pos_h)*cohesion_constant;
            sp->speed.h += speed_h * align_constant;
    
            sp->speed.v += (group_v - current_sheep_pos_v)*cohesion_constant;
            sp->speed.v += speed_v * align_constant;

        } else {

            group_h = tot_h/tot_sprites;
            group_v = tot_v/tot_sprites;

            speed_h = tot_speed_h / tot_sprites;
            speed_v = tot_speed_v / tot_sprites;
             


            sp->speed.h += (group_h - current_sheep_pos_h)*cohesion_constant;
            //sp->speed.h += speed_h * align_constant;
    
            sp->speed.v += (group_v - current_sheep_pos_v)*cohesion_constant;
            //sp->speed.v += speed_v * align_constant;

            //sp->speed.h += (random() % 100  / 100.0f)*max_speed/10;
            //sp->speed.v += (random() % 100  / 100.0f)*max_speed/10;
        }

        
        //Normalize speed
        //
    
        if (sp->face != blackFace) {

            normal_speed = sqrt(pow(sp->speed.h, 2.0) + pow(sp->speed.v,2.0));
            sp->speed.h = (sp->speed.h/normal_speed) * max_speed;
            sp->speed.v = (sp->speed.v/normal_speed) * max_speed;
        } else {

            normal_speed = sqrt(pow(sp->speed.h, 2.0) + pow(sp->speed.v,2.0));
            sp->speed.h = (sp->speed.h/normal_speed) * max_speed*1.4; //(max_speed + max_speed/2-(random() % 100  / 100.0f)*max_speed);
            sp->speed.v = (sp->speed.v/normal_speed) *max_speed*1.4;

        } 



        if (sp->face == blackFace) {

            //sp->speed.h += max_speed/2-(random() % 100  / 100.0f)*max_speed ;
            //sp->speed.v += max_speed/2-random(max_speed);


            //printf("black sheep detected!\n");


        }

		sp = sp->next;
	} while (sp != NULL);
}


float avoidanceCalc(float input, float distance) {

    return input / distance;  

}


// Drawing routine
void Display()
{
	SpritePtr sp;
	
	glClearColor(0, 0, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	DrawBackground();
	
	SpriteBehavior(); // Din kod!
	
// Loop though all sprites. (Several loops in real engine.)
	sp = gSpriteRoot;
	do
	{
		HandleSprite(sp); // Callback in a real engine
		DrawSprite(sp);
		sp = sp->next;
	} while (sp != NULL);
	
	glutSwapBuffers();
}

void Reshape(int h, int v)
{
	glViewport(0, 0, h, v);
	gWidth = h;
	gHeight = v;
}

void Timer(int value)
{
	glutTimerFunc(20, Timer, 0);
	glutPostRedisplay();
}

// Example of user controllable parameter
float someValue = 0.0;

void Key(unsigned char key,
         __attribute__((unused)) int x,
         __attribute__((unused)) int y)
{
  switch (key)
  {

float vseparation_dist_sqr = 10000.0;
//float group_dist_sqr = 40000.0; // squared distance




    case 't':
    	separation_dist_sqr  += 100;
    	printf("separation_dist_sqr  = %f\n", separation_dist_sqr);
    	break;
    case 'g':
    	separation_dist_sqr  -= 100;
    	printf("separation_dist_sqr  = %f\n", separation_dist_sqr);
        break;
    case 'r':
    	group_dist_sqr += 100;
    	printf("group_dist_sqr = %f\n", group_dist_sqr);
    	break;
    case 'f':
        group_dist_sqr -= 100;
    	printf("group_dist_sqr = %f\n", group_dist_sqr);
    	break;


    case 'q':
    	cohesion_constant += 0.001;
    	printf("cohesion_constant = %f\n", cohesion_constant);
    	break;
    case 'a':
    	cohesion_constant -= 0.001;
    	printf("cohesion_constant = %f\n", cohesion_constant);
    	break;
    case 'w':
    	separation_constant += 0.100;
    	printf("separation_constant = %f\n", separation_constant);
    	break;
    case 's':
    	separation_constant -= 0.100;
    	printf("separation_constant = %f\n", separation_constant);
    	break;
    case 'e':
    	align_constant += 0.01;
    	printf("align_constant = %f\n", align_constant);
    	break;
    case 'd':
    	align_constant -= 0.01;
    	printf("align_constant = %f\n", align_constant);
    	break;
    case 0x1b:
      exit(0);
  }
}

void Init()
{	
	LoadTGATextureSimple("bilder/leaves.tga", &backgroundTexID); // Bakgrund
	
	sheepFace = GetFace("bilder/sheep.tga"); // Ett fr
	blackFace = GetFace("bilder/blackie.tga"); // Ett svart fr
	dogFace = GetFace("bilder/dog.tga"); // En hund
	foodFace = GetFace("bilder/mat.tga"); // Mat
	
    NewSprite(blackFace, 256, 256, 1, 1.6);
	NewSprite(sheepFace, 700, 200, 1, 1.2);
	NewSprite(sheepFace, 200, 500, 1.6, -1);
	NewSprite(sheepFace, 250, 200, -1.8, 0.3);
	NewSprite(sheepFace, 50, 550, 1, 1);
	NewSprite(sheepFace, 300, 100, 1.5, -0.8);
	NewSprite(sheepFace, 450, 200, -1, 1.7);
	NewSprite(sheepFace, 400, 300, 1, 1.7);
	NewSprite(sheepFace, 300, 50, 1.4, -1);
	NewSprite(sheepFace, 250, 5, -1.2, 1.5);
	NewSprite(sheepFace, 700, 500, -1, 1.2);
	NewSprite(sheepFace, 200, 700, 1.6, -1);
	NewSprite(sheepFace, 250, 600, -1.8, 0.3);
	NewSprite(sheepFace, 500, 550, 1, 1);
	NewSprite(sheepFace, 300, 300, 1.5, -0.8);
	NewSprite(sheepFace, 400, 400, -1, 1.7);
	NewSprite(sheepFace, 300, 400, 1, 0.9);
	NewSprite(sheepFace, 100, 50, 1.4, -1);
	NewSprite(sheepFace, 450, 5, 1.2, 1.5);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SpriteLight demo / Flocking");
	
	glutDisplayFunc(Display);
	glutTimerFunc(20, Timer, 0); // Should match the screen synch
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);
	
	InitSpriteLight();
	Init();
	
	glutMainLoop();
	return 0;
}
