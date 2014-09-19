#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/glut.h>
#include "GPGPU.h"

void reshape(int w, int h);

GPGPU  *gpgpu;

// GLUT idle function
void idle()
{
    glutPostRedisplay();
}

// GLUT display function
void display()
{
	// Update the scene
    gpgpu->update();  

	// Display the results
    gpgpu->display();
    glutSwapBuffers();
}

// GLUT reshape function
void reshape(int w, int h)
{
    if (h == 0) h = 1;
    
    glViewport(0, 0, w, h);
    
	// Use orthographic projection
    glMatrixMode(GL_PROJECTION);    
    glLoadIdentity();               
    gluOrtho2D(-1, 1, -1, 1);       
    glMatrixMode(GL_MODELVIEW);     
    glLoadIdentity();               
}

// Called at startup
void initialize()
{
    // Initialize glew library
    glewInit();

    // Create the gpgpu object
    gpgpu = new GPGPU(512, 512);
}

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
