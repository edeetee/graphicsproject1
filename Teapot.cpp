//  ========================================================================
//  COSC363: Computer Graphics (2015);  University of Canterbury.
//
//  FILE NAME: Teapot.cpp
//  See Lab01.pdf for details
//  ========================================================================

#include <cmath>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>

#include "loadTGA.h"

#define PI 3.14159265

float camx = 0;
float camy = 0;
float camz = 20;

float aimx = 0;
float aimy = 0;
float aimz = 1;

float move = 0;

GLuint skyleft;
GLuint skyfront;
GLuint skyright;
GLuint skyback;
GLuint skyup;
GLuint skydown;
GLuint concrete;

void computeNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
	float nx, ny, nz;
	nx = y1*(z2-z3) + y2*(z3-z1) + y3*(z1-z2);
	ny = z1*(x2-x3) + z2*(x3-x1) + z3*(x1-x2);
	nz = x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2);
	glNormal3f(nx, ny, nz);
}

//------Function to load a texture in bmp format  ------------------------
GLuint loadTexture(char* file)				
{
	GLuint txId;
	glGenTextures(1, &txId); 				// Create a Texture object
	glBindTexture(GL_TEXTURE_2D, txId);		//Use this texture
    loadTGA(file);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	return txId;
}

void loadTextures(){
	skyleft = loadTexture("skybox/left.tga");
	skyfront = loadTexture("skybox/front.tga");
	skyright = loadTexture("skybox/right.tga");
	skyback = loadTexture("skybox/back.tga");
	skyup = loadTexture("skybox/up.tga");
	skydown = loadTexture("skybox/down.tga");

	concrete = loadTexture("Concrete.tga");
}

//--Draws a grid of lines on the floor plane -------------------------------
void drawFloor()
{
	glColor3f(1., 1.,  1.);			//Floor colour
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, concrete);
	glBegin(GL_QUADS);
		glTexCoord2f(0.,0.); glVertex3f(-50,-10,-50);
		glTexCoord2f(0.,1.); glVertex3f(-50,-10,50);
		glTexCoord2f(1.,1.); glVertex3f(50,-10,50);
		glTexCoord2f(1.,0.); glVertex3f(50,-10,-50);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawCylinder(float angles, float y_separator, int num_radiuses, float* radiuses){

	float angle_separation = 2 * PI / angles;

	glBegin(GL_QUADS);
	for(int y = 0; y < num_radiuses-1; y ++){
		float radius = radiuses[y];
		float next_radius = radiuses[y+1];
		for(int x = 0; x < angles; x++){
			computeNormal(radius*cos(x*angle_separation), y_separator*y, radius*sin(x*angle_separation), 
				next_radius*cos(x*angle_separation), y_separator*(y+1), next_radius*sin(x*angle_separation),
				radius*cos((x+1)*angle_separation), y_separator*y, radius*sin((x+1)*angle_separation));

			glVertex3f(radius*cos(x*angle_separation), y_separator*y, radius*sin(x*angle_separation));
			glVertex3f(next_radius*cos(x*angle_separation), y_separator*(y+1), next_radius*sin(x*angle_separation));
			glVertex3f(next_radius*cos((x+1)*angle_separation), y_separator*(y+1), next_radius*sin((x+1)*angle_separation));
			glVertex3f(radius*cos((x+1)*angle_separation), y_separator*y, radius*sin((x+1)*angle_separation));
		}
	}
	glEnd();
}

void drawSpaceship(){
	int curTime = glutGet(GLUT_ELAPSED_TIME);

	glColor3f(0.8, 0.8, 0.8);
	float rad1[] = {0, 4, 7, 8, 8, 7, 4};
	int num_rad1 = 7;
    drawCylinder(10, 0.5, num_rad1, rad1);

    glPushMatrix();
    	glTranslatef(0, (num_rad1-1)*0.5, 0);
	    glColor3f(0.2, 0.2, 0.2);
	    float rad2[] = {4, 2, 2, 0};
	    drawCylinder(10, 0.5, 4, rad2);
    glPopMatrix();

    glPushMatrix();
		int hoverPeriod = 200;
		float hoverMod = (sin(curTime/(float)hoverPeriod)+1)/2;
		glScalef(10,10,10);
		glTranslatef(0,0.04,0);

		glRotatef(90,1,0,0);
		glTranslatef(0,0,0.005 + 0.05*hoverMod);
		glColor3f(.9,.3,.3);
		glutSolidTorus(0.005, 0.4, 10, 20);

		glTranslatef(0,0,0.01 + 0.05*hoverMod);
		glColor3f(.6,.1,.1);
		glutSolidTorus(0.0014, 0.3, 6, 20);

		glTranslatef(0,0,0.015 + 0.05*hoverMod);
		glColor3f(.4,0,0);
		glutSolidTorus(0.0008, 0.2, 4, 20);
	glPopMatrix();
}

void drawAlien(){
	//head
	int curTime = glutGet(GLUT_ELAPSED_TIME);

	glColor3f(0,0.8,0);
	glPushMatrix();
		glTranslatef(0, 1.5, 0);
		//solid head
		glPushMatrix();
			glScalef(0.2, 0.2, 0.2);
			glutSolidDodecahedron();
		glPopMatrix();

		//eyes
		glPushMatrix();
			glColor3f(0,0,0);
			glTranslatef(0.2, 0, 0.2);
			glutSolidSphere(0.1, 5, 5);
			glTranslatef(-0.4, 0, 0);
			glutSolidSphere(0.1, 5, 5);
		glPopMatrix();

		glColor3f(0,0.9,0.4);
		glRotatef(-90,1,0,0);
		glRotatef(20,0,1,0);
		int conePeriod = 500;
		float coneMod = (sin(curTime/(float)conePeriod)+1)/2;
		glutSolidCone(0.15, -0.2*coneMod+0.7, 6, 10);
		glRotatef(-40,0,1,0);
		glutSolidCone(0.15, 0.2*coneMod+0.5, 6, 10);
	glPopMatrix();

	glPushMatrix();
	//
	glPopMatrix();

	glPushMatrix();
		glColor3f(0,0.6,0);
		int bodyPeriod = 420;
		float bodyMod = (sin(curTime/(float)bodyPeriod)+1)/2;

		float rad[] = {0, 0.2, 0.4, -0.1*bodyMod+0.3, -0.15*bodyMod+0.3, -0.1*bodyMod+0.3, 0.4, -0.2*bodyMod+0.5, -0.3*bodyMod+0.55, 0.4, -0.1*bodyMod+0.4, -0.15*bodyMod+0.4, -0.1*bodyMod+0.4, 0.2, 0};
		int num_rad = 15;
	    drawCylinder(8, 0.1, num_rad, rad);
	    
	glPopMatrix();

	glPushMatrix();
		int hoverPeriod = 200;
		float hoverMod = (sin(curTime/(float)hoverPeriod)+1)/2;

		glRotatef(90,1,0,0);
		glTranslatef(0,0,0.05 + 0.05*hoverMod);
		glColor3f(.9,.3,.3);
		glutSolidTorus(0.1, 0.2, 10, 6);

		glTranslatef(0,0,0.1 + 0.05*hoverMod);
		glColor3f(.6,.1,.1);
		glutSolidTorus(0.07, 0.15, 6, 4);

		glTranslatef(0,0,0.15 + 0.05*hoverMod);
		glColor3f(.4,0,0);
		glutSolidTorus(0.04, 0.1, 4, 4);
	glPopMatrix();
}

void drawSkyPlane(GLuint txId){
 	glBindTexture(GL_TEXTURE_2D, txId);
 	glBegin(GL_QUADS);
 		glTexCoord2f(1.,1.); glVertex3f(-5,-5,0);
		glTexCoord2f(0.,1.); glVertex3f(5,-5,0);
		glTexCoord2f(0.,0.); glVertex3f(5,5,0);
		glTexCoord2f(1.,0.); glVertex3f(-5,5,0);
	glEnd();
}

void drawSkybox(){
	glEnable(GL_TEXTURE_2D);
	glColor3f(1,1,1);

	glPushMatrix();
		glTranslatef(0,5,0);
		glRotatef(90,1,0,0);
		glRotatef(90,0,0,1);
		drawSkyPlane(skyup);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0,-5,0);
		glRotatef(-90,0,1,0);
		glRotatef(-90,1,0,0);
		drawSkyPlane(skydown);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-5,0,0);
		glRotatef(90,0,1,0);
		drawSkyPlane(skyleft);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0,0,-5);
		drawSkyPlane(skyfront);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(5,0,0);
		glRotatef(-90,0,1,0);
		drawSkyPlane(skyright);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0,0,5);
		glRotatef(180,0,1,0);
		drawSkyPlane(skyback);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void updateCamera(){
	camx += move*aimx;
	camy += move*aimy;
	camz += move*aimz;
}

void drawPatrol(){
	static int phaseStart = 0;
	static float rotation = 0;
	static float position = 0;
	int curTime = glutGet(GLUT_ELAPSED_TIME);
	float phaseTime = curTime - phaseStart;

	if(phaseTime < 10000){
		position = 20*phaseTime/10000;
	} else if(phaseTime < 11000){
		rotation = 180*(phaseTime-10000)/1000;
	} else if(phaseTime < 21000){
		position = 20-20*(phaseTime-11000)/10000;
	} else if(phaseTime < 22000){
		rotation = 180 - 180*(phaseTime-21000)/1000;
	} else{
		phaseStart = curTime;
	}

	glPushMatrix();
		glTranslatef(20, -9, position);
		glRotatef(rotation, 0, 1, 0);
    	drawAlien();
    	glTranslatef(-1,0,-1);
    	drawAlien();
    	glTranslatef(2, 0, 0);
    	drawAlien();
	glPopMatrix();
}

void drawHighSpaceship(int x, int y, float period){
	int curTime = glutGet(GLUT_ELAPSED_TIME);

	glPushMatrix();
    	glTranslatef(sin(curTime/period)*x, 0, cos(curTime/period)*y);
    	drawSpaceship();
	glPopMatrix();
}

//--Display: ---------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display(void) 
{ 
	updateCamera();

	float lpos[4] = {10., 10., 10., 0};  //light's position

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_LIGHTING);
		glPushMatrix();
		    gluLookAt(0, 0, 0, aimx, aimy, aimz, 0, 1, 0);  //Camera position and orientation
		    glDisable(GL_DEPTH_TEST);
	    	drawSkybox();
	    	glEnable(GL_DEPTH_TEST);
    	glPopMatrix();

    	gluLookAt(camx, camy, camz, camx+aimx, camy+aimy, camz+aimz, 0, 1, 0);  //Camera position and orientation
	    drawFloor();
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //Set light position

    glPushMatrix();
    	glTranslatef(20, 0, 0);
    	drawSpaceship();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(100, 10, 200);
		drawHighSpaceship(200, 200, 2000);
		glTranslatef(-200, 100, -200);
		drawHighSpaceship(1000, 1000, 10000);
		glTranslatef(100, 50, 0);
		drawHighSpaceship(100, 100, 1000);
		glTranslatef(0, 500, 200);
		drawHighSpaceship(1000, 100, 5000);
	glPopMatrix();
	//Special ufo camera
	glPushMatrix();
		glTranslatef(0, 500, 0);
        drawHighSpaceship(100, 100, 1000);
	glPopMatrix();

	drawPatrol();

	glFlush(); 

	glutPostRedisplay();
} 

//for keys and stuff
void special(int key, int x, int y)
{
	switch(key){
		case GLUT_KEY_UP:
			move = 0.1;
			break;
		case GLUT_KEY_DOWN:
			move = -0.1;
			break;
		default:
			break;
	}
}

void special_up(int key, int x, int y)
{
	switch(key){
		default:
			move = 0;
			break;
	}
}

void keyboard(unsigned char key, int x, int y){
	switch(key){
		case 'w':
			move = 0.1;
			break;
		case 's':
			move = -0.1;
			break;

	}
}

void keyboard_up(unsigned char key, int x, int y){
	switch(key){
		default:
			move = 0;
	}
}

//mouse movement
void passiveMotion(int x, int y){
	float fx = -(float)x / 50;
	float fy = -(float)y / 800 * PI + PI/2;

	aimx = cos(fy) * sin(fx);
	aimy = sin(fy);
	aimz = cos(fy) * cos(fx);
}

//----------------------------------------------------------------------
void initialize(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_LIGHTING);		//Enable OpenGL states
	glEnable(GL_LIGHT0);
 	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	loadTextures();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65, 1, 1.0, 1000.0);   //Camera Frustum
}


//  ------- Main: Initialize glut window and register call backs -------
int main(int argc, char **argv) 
{ 
	glutInit(&argc, argv);            
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);  
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	
	glutCreateWindow("Teapot");
	initialize();
	glutDisplayFunc(display);

	glutSpecialFunc(special);
	glutSpecialUpFunc(special_up);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_up);

	glutPassiveMotionFunc(passiveMotion);

	glutMainLoop();
	return 0; 
}
