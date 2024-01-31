#ifndef RENDERER_H
#define RENDERER_H

#define GLrenderCall(x) glcheckerrors();\
x;\
glLogCall(#x, __FILE__, __LINE__);\
{ if((glGetError()) != (GL_NO_ERROR)) {break;} }


#define GLcall(x) glcheckerrors();\
x;\
glLogCall(#x, __FILE__, __LINE__)

//#include "GLEW/GL/glew.h"
#include "GLAD/include/glad/glad.h"
#include "GLFW/glfw3.h"



void glLogCall(const char* Function, const char* file, unsigned int line);
void glcheckerrors();



#endif // RENDERER_H
