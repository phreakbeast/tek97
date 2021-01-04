#ifndef _TEK_PLATFORM_HPP
#define _TEK_PLATFORM_HPP

#include "../core/tek_core.hpp"

//---------------------------------------------------
// opengl
//---------------------------------------------------
#ifdef TEK_PLATFORM_LINUX
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <GL/glxext.h>
#endif

#ifdef TEK_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLext.h>
#define WGL_WGLEXT_PROTOTYPES
#include <GL/wglext.h>
#endif

#ifdef TEK_DEBUG
#define GLCall(x) x; _check_gl_error(#x, __FILE__, __LINE__)
#else
#define GLCall(x) x
#endif

void _check_gl_error(const char *function, const char *file, int line);

void print_gl_string(GLenum name, const char *label);

#define GL_FUNCTION(x, y) extern PFN## x ##PROC y;

#include "gl_functions.inl"

void load_gl_extensions();

//---------------------------------------------------
// window
//---------------------------------------------------

typedef enum
{
	INPUT_LETTER_LETTER,
	INPUT_LETTER_BACKSPACE,
	INPUT_LETTER_ESCAPE,
	INPUT_LETTER_TAB,
	INPUT_LETTER_ENTER,
	INPUT_LETTER_SPACE,
	INPUT_LETTER_UP,
	INPUT_LETTER_DOWN,
	INPUT_LETTER_LEFT,
	INPUT_LETTER_RIGHT,
	INPUT_LETTER_DELETE,
	INPUT_LETTER_NONE,
}TekInputLetterType;

typedef struct
{
	TekInputLetterType type;
	Key key;
}TekInputLetter;

void tek_window_destroy();

bool tek_window_open(u32 width, u32 height, const char *title, bool fullscreen);

//TODO: implement
void tek_window_toggle_fullscreen();

void tek_window_swap_buffers();

bool tek_window_update();

bool tek_window_should_close();

bool tek_window_is_key_down(Key key);

bool tek_window_is_mouse_down(MouseButton button);

int tek_window_get_mouse_x();

int tek_window_get_mouse_y();

int tek_window_get_mouse_wheel();

void tek_window_set_cursor_pos(int x, int y);

void tek_window_show_cursor(int val);

TekInputLetter *tek_window_get_letter(int *letters);

//---------------------------------------------------
// timing
//---------------------------------------------------

void tek_time_init();

double tek_time_get_seconds();

//TODO: implement
void tek_time_sleep_ms(int milliseconds);

#endif