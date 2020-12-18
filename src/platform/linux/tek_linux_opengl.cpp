#include "../tek_platform.h"

#undef GL_FUNCTION
#define GL_FUNCTION(x, y)PFN## x ##PROC y = NULL;

#include "../gl_functions.inl"
#include <stdio.h>

void load_gl_extensions()
{
#ifdef ARGOS_PLATFORM_LINUX
#undef GL_FUNCTION
#define GL_FUNCTION(x, y)y = (PFN## x ##PROC)glXGetProcAddress((const GLubyte*)#y);

#include "../gl_functions.inl"

#endif

#ifdef ARGOS_PLATFORM_WINDOWS
#undef GL_FUNCTION
#define GL_FUNCTION(x,y)y = (PFN## x ##PROC)wglGetProcAddress(#y);

#include "../gl_functions.inl"
#endif

//TODO: some kind of check if the functions pointers are correct
}

void _check_gl_error(const char *function, const char *file, int line)
{
	GLenum err = glGetError();
	while (err != GL_NO_ERROR)
	{
		const char *error = "";
		switch (err)
		{
			case GL_INVALID_OPERATION:
				error = "INVALID_OPERATION";
				break;
			case GL_INVALID_ENUM:
				error = "INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:
				error = "INVALID_VALUE";
				break;
			case GL_OUT_OF_MEMORY:
				error = "OUT_OF_MEMORY";
				break;
				/*case GL_INVALID_FRAMEBUFFER_OPERATION:
				  error = "INVALID_FRAMEBUFFER_OPERATION";
				  break;*/
			default:
				break;
		}

		printf("GL_%s - %s \n%s:%i\n", error, function, file, line);
		err = glGetError();
		//assert(0);
	}
}

void print_gl_string(GLenum name, const char *label)
{
	const GLubyte *ret = glGetString(name);
	if (ret == 0)
	{
		printf("Error getting string: %i\n", name);
	}
	else
	{
		printf("%s: %s\n", label, ret);
	}
}