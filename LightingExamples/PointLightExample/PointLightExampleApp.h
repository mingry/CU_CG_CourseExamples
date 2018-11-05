#pragma once

void Reshape( int w, int h );


// Keyboard and Mouse
void Keyboard(unsigned char key, int x, int y );
void SpeicalKeyboard(int key, int x, int y );
void Mouse(int button, int state, int x, int y);
void MouseMotion(int x, int y);




// OpenGL
void InitOpenGL();
void Display();
void ClearOpenGLResource();
