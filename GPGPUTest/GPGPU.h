#pragma once

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/glut.h>
#include <string>

class GPGPU
{
public:
	GPGPU(int w, int h);

	void update();
	void display();
	int loadShader(char* filename, std::string& text);

private:
    int _iWidth, _iHeight;	// The dimensions of the data array
	int _initialized;
	GLuint _initializedLoc;
    GLuint _texLoc;		// a parameter to the fragment program
    
    GLuint _textureId;		// The texture ID used to store data array
    GLuint _programId;		// the program ID
    GLuint _fragmentShader;
};

