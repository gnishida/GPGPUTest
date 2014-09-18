//      1.) Texture = Array
//      2.) Fragment Program = Computational Kernel.
//      3.) One-to-one Pixel to Texel Mapping: 
//          a) Data-Dimensioned Viewport, and
//          b) Orthographic Projection.
//      4.) Viewport-Sized Quad = Data Stream Generator.
//      5.) Copy To Texture = feedback.
//


#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string>
#include "GPGPU.h"

void reshape(int w, int h);

// globals
GPGPU  *g_pHello;

int loadshader(char* filename, std::string& text)
{
	std::ifstream ifs;
	ifs.open(filename, std::ios::in);

	std::string line;
	while (ifs.good()) {
        getline(ifs, line);

		text += line + "\n";
    }

	return 0;
}

// GLUT idle function
void idle()
{
    glutPostRedisplay();
}

// GLUT display function
void display()
{
    g_pHello->update();  // update the scene and run the edge detect filter
    g_pHello->display(); // display the results
    glutSwapBuffers();
}

// GLUT reshape function
void reshape(int w, int h)
{
    if (h == 0) h = 1;
    
    glViewport(0, 0, w, h);
    
    // GPGPU CONCEPT 3b: One-to-one Pixel to Texel Mapping: An Orthographic 
    //                   Projection.
    // This code sets the projection matrix to orthographic with a range of 
    // [-1,1] in the X and Y dimensions. This allows a trivial mapping of 
    // pixels to texels.
    glMatrixMode(GL_PROJECTION);    
    glLoadIdentity();               
    gluOrtho2D(-1, 1, -1, 1);       
    glMatrixMode(GL_MODELVIEW);     
    glLoadIdentity();               
}

// Called at startup
void initialize()
{
    // Initialize the "OpenGL Extension Wrangler" library
    glewInit();

    // Create the example object
    g_pHello = new GPGPU(512, 512);
}

// The main function
int main()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(512, 512);
    glutCreateWindow("GPU programming test");

    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    initialize();

    glutMainLoop();

    return 0;
}
