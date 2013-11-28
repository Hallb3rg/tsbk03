
// Converted to MicroGlut and VectorUtils3 2013.
// MicroGLut currently exists for Linux and Mac OS X. On other platforms (e.g. MS Windows)
// you should use FreeGLUT. You will also need GLEW. You may sometimes need to work around
// differences, e.g. additions in MicroGlut that don't exist in FreeGlut.

// gcc lab0.c ../common/*.c -lGL -o lab0 -I../common

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	//uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "zpr.h"
#include "LoadTGA.h"

//constants
const int initWidth=512,initHeight=512;

// Modify this matrix.
// See below for how it is applied to your model.
mat4 objectExampleMatrix = {{ 1.0, 0.0, 0.0, 0.0,
                            0.0, 1.0, 0.0, 0.0,
                            0.0, 0.0, 1.0, 0.0,
                            0.0, 0.0, 0.0, 1.0}};

mat4 viewMatrix, projectionMatrix;

// Globals
// * Model(s)
Model *bunny;
Model *teddy;
// * Reference(s) to shader program(s)
GLuint program;
// * Texture(s)
GLuint texture;
GLuint texture2;

void OnTimer(int value);



void init(void)
{

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	printError("GL inits");

	projectionMatrix = perspective(90, 1.0, 0.1, 1000);

	// Load and compile shader
	program = loadShaders("lab0.vert", "lab0.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	bunny = LoadModelPlus("objects/bilskiss.obj");
    teddy = LoadModelPlus("objects/teapot.obj");
	printError("load models");

	// Load textures
	LoadTGATextureSimple("textures/maskros512.tga",&texture);
    LoadTGATextureSimple("textures/bilskissred.tga", &texture2);
    
    glActiveTexture(GL_TEXTURE0);//which texture-unit is active

    glBindTexture(GL_TEXTURE_2D, texture);//load the texture to active texture-unit

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

	printError("load textures");

    glutTimerFunc(20, &OnTimer, 0);

}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//activate the program, and set its variables
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	mat4 m = Mult(viewMatrix, objectExampleMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, m.m);
    
    glUniform1i(glGetUniformLocation(program,"exampletexture"),1);

    
	//draw the model
	DrawModel(bunny, program, "in_Position", "in_Normal", "in_TexCoord");

    glUniform1i(glGetUniformLocation(program,"exampletexture"),0);
    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

    mat4 rot = Ry(t*0.001);
    mat4 trans = T(0,0,-5);
    mat4 scale = S(0.9,0.9,0.9);
    mat4 m2 = Mult(viewMatrix, objectExampleMatrix);
    m2 = Mult(m2, scale);
    m2 = Mult(m2, rot);
    m2 = Mult(m2, trans);



    glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, m2.m);

    DrawModel(teddy, program, "in_Position", "in_Normal", "in_TexCoord");

	printError("display");
	
	glutSwapBuffers();
}

void idle()
{
  // This function is called whenever the computer is idle
  // As soon as the machine is idle, ask GLUT to trigger rendering of a new frame
  glutPostRedisplay();
}

vec3 cam = {0,0,2}, point = {0,0,0};

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("Lab 0 - OpenGL 3.0 Introduction");
	zprInit(&viewMatrix, cam, point);
	glutDisplayFunc(display); 
	glutIdleFunc(idle);
	init ();
	glutMainLoop();
}

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}


