#include "GPGPU.h"
#include <iostream>
#include <fstream>

GPGPU::GPGPU(int w, int h) : _rAngle(0), _hoge(0), _iWidth(w), _iHeight(h)
{
    // Create a simple 2D texture.  This example does not use
    // render to texture -- it just copies from the framebuffer to the
    // texture.

    // GPGPU CONCEPT 1: Texture = Array.
    // Textures are the GPGPU equivalent of arrays in standard 
    // computation. Here we allocate a texture large enough to fit our
    // data (which is arbitrary in this example).
    glGenTextures(1, &_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _iWidth, _iHeight, 0, GL_RGB, GL_FLOAT, 0);

    _programId = glCreateProgram();

	// load fragment shader which will be used as computational kernel
	std::string edgeFragSource2;
	loadshader("fragment.glsl", edgeFragSource2);

    // Create the edge detection fragment program
    _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* source = edgeFragSource2.c_str();
    glShaderSource(_fragmentShader, 1, &source, NULL);
    glCompileShader(_fragmentShader);
    glAttachShader(_programId, _fragmentShader);

    // Link the shader into a complete GLSL program.
    glLinkProgram(_programId);

	// Check program
	{
		int infologLength = 0;
		glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &infologLength);
		if (infologLength > 0) {
			char *infoLog = (char *)malloc(infologLength);
			glGetProgramInfoLog(_programId, infologLength, NULL, infoLog);
			printf("%s\n",infoLog);
			free(infoLog);
		}
	}

    // Get location of the uniform variables
    _texUnit = glGetUniformLocation(_programId, "texUnit");
	_hoge = glGetUniformLocation(_programId, "hoge");
}

void GPGPU::update()
{
    _rAngle += 0.5f;

    // store the window viewport dimensions so we can reset them,
    // and set the viewport to the dimensions of our texture
    int vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    // GPGPU CONCEPT 3a: One-to-one Pixel to Texel Mapping: A Data-
    //                   Dimensioned Viewport.
    // We need a one-to-one mapping of pixels to texels in order to 
    // ensure every element of our texture is processed. By setting our 
    // viewport to the dimensions of our destination texture and drawing 
    // a screen-sized quad (see below), we ensure that every pixel of our 
    // texel is generated and processed in the fragment program.
    glViewport(0, 0, _iWidth, _iHeight);
        
    // Render a teapot and 3 tori
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glRotatef(-_rAngle, 0, 1, 0.25);
    glutSolidTeapot(0.5);
    glPopMatrix();
    glPushMatrix();
    glRotatef(2.1 * _rAngle, 1, 0.5, 0);          
    glutSolidTorus(0.05, 0.9, 64, 64);
    glPopMatrix();
    glPushMatrix();
    glRotatef(-1.5 * _rAngle, 0, 1, 0.5);
    glutSolidTorus(0.05, 0.9, 64, 64);
    glPopMatrix();
    glPushMatrix();
    glRotatef(1.78 * _rAngle, 0.5, 0, 1);
    glutSolidTorus(0.05, 0.9, 64, 64);
    glPopMatrix();
        
    // copy the results to the texture
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, _iWidth, _iHeight);
        

    // run the edge detection filter over the geometry texture
    // Activate the edge detection filter program
    glUseProgram(_programId);
            
    // identify the bound texture unit as input to the filter
    glUniform1i(_texUnit, 0);
	float r = (float)(rand() % 100) * 0.01f;
	glUniform1f(_hoge, r);
            
    // GPGPU CONCEPT 4: Viewport-Sized Quad = Data Stream Generator.
    // In order to execute fragment programs, we need to generate pixels.
    // Drawing a quad the size of our viewport (see above) generates a 
    // fragment for every pixel of our destination texture. Each fragment
    // is processed identically by the fragment program. Notice that in 
    // the reshape() function, below, we have set the frustum to 
    // orthographic, and the frustum dimensions to [-1,1].  Thus, our 
    // viewport-sized quad vertices are at [-1,-1], [1,-1], [1,1], and 
    // [-1,1]: the corners of the viewport.
    glBegin(GL_QUADS);
    {            
        glTexCoord2f(0, 0); glVertex3f(-1, -1, -0.5f);
        glTexCoord2f(1, 0); glVertex3f( 1, -1, -0.5f);
        glTexCoord2f(1, 1); glVertex3f( 1,  1, -0.5f);
        glTexCoord2f(0, 1); glVertex3f(-1,  1, -0.5f);
    }
    glEnd();
        
    // disable the filter
    glUseProgram(0);
        
    // GPGPU CONCEPT 5: Copy To Texture (CTT) = Feedback.
    // We have just invoked our computation (edge detection) by applying 
    // a fragment program to a viewport-sized quad. The results are now 
    // in the frame buffer. To store them, we copy the data from the 
    // frame buffer to a texture.  This can then be fed back as input
    // for display (in this case) or more computation (see 
    // more advanced samples.)

    // update the texture again, this time with the filtered scene
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, _iWidth, _iHeight);
        
    // restore the stored viewport dimensions
    glViewport(vp[0], vp[1], vp[2], vp[3]);
}

void GPGPU::display()
{
    // Bind the filtered texture
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glEnable(GL_TEXTURE_2D);

    // render a full-screen quad textured with the results of our 
    // computation.  Note that this is not part of the computation: this
    // is only the visualization of the results.
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0); glVertex3f(-1, -1, -0.5f);
        glTexCoord2f(1, 0); glVertex3f( 1, -1, -0.5f);
        glTexCoord2f(1, 1); glVertex3f( 1,  1, -0.5f);
        glTexCoord2f(0, 1); glVertex3f(-1,  1, -0.5f);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

int GPGPU::loadshader(char* filename, std::string& text)
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