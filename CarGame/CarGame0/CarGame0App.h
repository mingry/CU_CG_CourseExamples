
#pragma once



//////////////////////////////////////////////////////////////////////
//// Declare callback functions
//////////////////////////////////////////////////////////////////////
void Reshape(int w, int h);
void Display();
void Keyboard(unsigned char key, int x, int y);
void KeyboardUp(unsigned char key, int x, int y);
void Timer(int value);
void Display();
void Mouse(int button, int state, int x, int y);
void MouseMotion(int x, int y);



////////////////////////////////////////////////////////////////////////////////////
//// OpenGL  
//////////////////////////////////////////////////////////////////////////////////////
void InitOpenGL();
void ClearOpenGLResource();


