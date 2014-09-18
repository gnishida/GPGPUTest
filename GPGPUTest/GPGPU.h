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
	int loadshader(char* filename, std::string& text);

private:
    int _iWidth, _iHeight;	// The dimensions of our array
    float _rAngle;			// used for animation
	float _hoge;
    
    GLuint _textureId;		// The texture used as a data array

    GLuint _programId;		// the program used to update
    GLuint _fragmentShader;

    GLuint _texUnit;		// a parameter to the fragment program
};

