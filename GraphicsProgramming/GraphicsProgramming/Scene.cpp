#include "Scene.h"
#include<iostream>

// Scene constructor, initilises OpenGL
// You should add further variables to need initilised.
Scene::Scene(Input *in)
{
	// Store pointer for input class
	input = in;
	initialiseOpenGL();

	// Other OpenGL / render setting should be applied here.
	

	// Initialise scene variables
	wireframe = false;
}

void Scene::handleInput(float dt)
{
	// Handle user input

	if (input->isKeyDown('r') && !wireframe)
	{
		glPolygonMode(GL_FRONT, GL_LINE);
		input->setKeyUp('r');
		wireframe = true;
	}

	if (input->isKeyDown('r') && wireframe)
	{
		glPolygonMode(GL_FRONT, GL_FILL);
		input->setKeyUp('r');
		wireframe = false;
	}	
}

void Scene::update(float dt)
{
	// update scene related variables.

	// Calculate FPS for output
	calculateFPS();
}

void drawSquare(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
	glBegin(GL_TRIANGLES);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z3);
		glVertex3f(x3, y3, z3);

		// Need to make sure these are drawn in an CCW fashion, turning on wireframe helped spot this originally.
		glVertex3f(x3, y3, z3);
		glVertex3f(-x2, -y2, z3);
		glVertex3f(x1, y1, z1);		
	glEnd();
}

void drawTriangleStrip()
{
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(0.5, 0, 0);
		glVertex3f(0.25, 0.5, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(-0.25, 0.5, 0);
		glVertex3f(-0.5, 0, 0);
		glVertex3f(-0.75, 0.5, 0);
		glVertex3f(-1.0, 0, 0);
	glEnd();
}

void drawTraingleFan()
{
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0, 0, 0);
		glVertex3f(1.0, 0.25, 0);
		glVertex3f(0.75, 0.5, 0);
		glVertex3f(0.5, 0.75, 0);
		glVertex3f(0.25, 1.0, 0);
		glVertex3f(0, 1.25, 0);
	glEnd();
}

void drawQuads()
{
	glBegin(GL_QUADS);
		// Quad 1
		glVertex3f(1.0, 0, 0);
		glVertex3f(1.0, 0.5, 0);
		glVertex3f(0.5, 1.25, 0);
		glVertex3f(0.5, 0.25, 0);

		// Quad 2
		glVertex3f(-1.0, 0, 0);
		glVertex3f(-1.0, -0.5, 0);
		glVertex3f(-0.5, -1.25, 0);
		glVertex3f(-0.5, -0.25, 0);
	glEnd();
}

void drawPolygon()
{
	glBegin(GL_POLYGON);
		glVertex3f(0, 0, 0);
		glVertex3f(1.0, -1.0, 0);
		glVertex3f(1.5, -0.5, 0);
		glVertex3f(1.5, 0.5, 0);
		glVertex3f(1.0, 1.0, 0);		
	glEnd();
}

void Scene::render()
{

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(0.0f, 0.0f, 6.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	//glColor3f(0.0, 1.0, 0.0);
	
	// Render geometry/scene here -------------------------------------

	/*glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0, 1.0, 0);

		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(-1.0, -1.0, 0);

		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(1.0, -1.0, 0);
	glEnd();*/

	//drawSquare(1.0, 1.0, 0, -1.0, 1.0, 0, -1.0, -1.0, 0);
	//drawTriangleStrip();
	//drawTraingleFan();
	//drawQuads();
	drawPolygon();

	// End render geometry --------------------------------------

	// Render text, should be last object rendered.
	renderTextOutput();
	
	// Swap buffers, after all objects are rendered.
	glutSwapBuffers();
}

void Scene::initialiseOpenGL()
{
	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.1f, 0.1f, 0.5f, 1.0f);				// Dark Blue Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);									// Clear stencil buffer
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
}

// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void Scene::resize(int w, int h) 
{
	width = w;
	height = h;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = (float)w / (float)h;
	fov = 45.0f;
	nearPlane = 0.1f;
	farPlane = 100.0f;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(fov, ratio, nearPlane, farPlane);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

// Calculates FPS
void Scene::calculateFPS()
{
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		sprintf_s(fps, "FPS: %4.2f", frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
}

// Compiles standard output text including FPS and current mouse position.
void Scene::renderTextOutput()
{
	// Render current mouse position and frames per second.
	sprintf_s(mouseText, "Mouse: %i, %i", input->getMouseX(), input->getMouseY());
	displayText(-0.985f, 0.95f, 1.f, 0.f, 0.f, mouseText);
	displayText(-0.985f, 0.90f, 1.f, 0.f, 0.f, fps);
}

// Renders text to screen. Must be called last in render function (before swap buffers)
void Scene::displayText(float x, float y, float r, float g, float b, char* string) {
	// Get Lenth of string
	int j = strlen(string);

	// Swap to 2D rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100); // Original
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Orthographic lookAt (along the z-axis).
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Set text colour and position.
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	// Render text.
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
	// Reset colour to white.
	glColor3f(1.f, 1.f, 1.f);

	// Swap back to 3D rendering.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, ((float)width/(float)height), nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}
