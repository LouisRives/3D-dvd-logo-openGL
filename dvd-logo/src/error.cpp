#include "error.h"
#include <iostream>

void glLogCall(const char* Function, const char* file, unsigned int line)
{
    while(GLenum error = glGetError())
    {
        std::cout << "code did not compile because" << " error code " << error << " occured in  " << Function << file << line << std::endl;
    }
}

void glcheckerrors()
{
    while(glGetError());
}
