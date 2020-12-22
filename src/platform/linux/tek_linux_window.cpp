#include "../tek_platform.hpp"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <stdio.h>
#include <assert.h>

static u32 g_width;
static u32 g_height;

typedef GLXContext(*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig, GLXContext, Bool, const int *);

static Display *g_display;
static Window g_window;
static GLXContext g_gc;

static Window g_root;
static XVisualInfo *g_vi;
static Colormap g_cmap;
static XSetWindowAttributes g_swa;
static bool g_keys_down[NUM_KEYS];
static bool g_buttons_down[NUM_MOUSE_BUTTONS];

static TekInputLetter g_input_letters[8];
static u32 g_num_input_letters = 0;

static Cursor g_blank_cursor;

static int g_mx = 0;
static int g_my = 0;
static int g_wheel = 0;

static bool g_close_requested = false;

static Key g_keys_defs[512];

static void init_keys();

static Key get_key_def(int key);

static Cursor create_blank_cursor()
{
	static char data[1] =
			{0};
	Cursor cursor;
	Pixmap blank;
	XColor dummy;

	blank = XCreateBitmapFromData(g_display, g_root, data, 1, 1);
	if (blank == None)
	{
		printf("Fatal Error: not enough memory for cursor\n");
	}
	cursor = XCreatePixmapCursor(g_display, blank, blank, &dummy, &dummy, 0, 0);
	XFreePixmap(g_display, blank);

	return cursor;
}

void tek_window_destroy()
{
	XFreeCursor(g_display, g_blank_cursor);
}

/*
int main(void) {
	int number_of_screens;
	int i;
	Bool result;
	Window *root_windows;
	Window window_returned;
	int root_x, root_y;
	int win_x, win_y;
	unsigned int mask_return;

	Display *display = XOpenDisplay(NULL);
	assert(display);
	XSetErrorHandler(_XlibErrorHandler);
	number_of_screens = XScreenCount(display);
	fprintf(stderr, "There are %d screens available in this X session\n", number_of_screens);
	root_windows = malloc(sizeof(Window) * number_of_screens);
	for (i = 0; i < number_of_screens; i++) {
		root_windows[i] = XRootWindow(display, i);
	}
	for (i = 0; i < number_of_screens; i++) {
		result = XQueryPointer(display, root_windows[i], &window_returned,
		                       &window_returned, &root_x, &root_y, &win_x, &win_y,
		                       &mask_return);
		if (result == True) {
			break;
		}
	}
	if (result != True) {
		fprintf(stderr, "No mouse found.\n");
		return -1;
	}
	printf("Mouse is at (%d,%d)\n", root_x, root_y);

	free(root_windows);
	XCloseDisplay(display);
	return 0;
}
 */

bool tek_window_open(u32 width, u32 height, const char *title, bool fullscreen)
{
	printf("creating window ...\n");

	g_width = width;
	g_height = height;

	g_display = XOpenDisplay(NULL);
	assert(g_display);

	int number_of_screens = XScreenCount(g_display);
	printf("number of screens %d\n", number_of_screens);

	g_root = DefaultRootWindow(g_display);

	GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};

	g_vi = glXChooseVisual(g_display, 0, att);
	assert(g_vi);

	g_cmap = XCreateColormap(g_display, g_root, g_vi->visual, AllocNone);

	g_swa.colormap = g_cmap;
	g_swa.event_mask =
			StructureNotifyMask | ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask |
			ButtonReleaseMask |
			PointerMotionMask | EnterWindowMask | LeaveWindowMask;

	g_window = XCreateWindow(g_display, g_root, 0, 0, g_width, g_height, 0,
	                         g_vi->depth, InputOutput, g_vi->visual, CWColormap | CWEventMask, &g_swa);

	//window manager should be able to close the window
	Atom del_window = XInternAtom(g_display, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(g_display, g_window, &del_window, 1);

	//for tiled window managers
	const char *wintype_name = "_NET_WM_WINDOW_TYPE_UTILITY";
	Atom _NET_WM_WINDOW_TYPE = XInternAtom(g_display, "_NET_WM_WINDOW_TYPE", False);
	Atom wintype = XInternAtom(g_display, wintype_name, False);
	XChangeProperty(g_display, g_window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32, PropModeReplace,
	                (unsigned char *) &wintype,
	                1);

	long compositor = 2;
	Atom _NET_WM_BYPASS_COMPOSITOR = XInternAtom(g_display, "_NET_WM_BYPASS_COMPOSITOR", False);
	XChangeProperty(g_display, g_window, _NET_WM_BYPASS_COMPOSITOR, XA_CARDINAL, 32, PropModeReplace,
	                (unsigned char *) &compositor, 1);

	printf("create opengl context\n");
	static int visual_attribs[] =
			{
					GLX_RENDER_TYPE, GLX_RGBA_BIT,
					GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
					GLX_DOUBLEBUFFER, true,
					GLX_RED_SIZE, 1,
					GLX_GREEN_SIZE, 1,
					GLX_BLUE_SIZE, 1,
					None
			};

	int num_fbc = 0;
	GLXFBConfig *fbc = glXChooseFBConfig(g_display, DefaultScreen(g_display), visual_attribs, &num_fbc);
	if (!fbc)
	{
		printf("Error: glXChoosevisual() failed\n");
		return false;
	}

	//create old OpenGL context
	XVisualInfo *vi = glXGetVisualFromFBConfig(g_display, fbc[0]);
	GLXContext ctx_old = glXCreateContext(g_display, vi, 0, GL_TRUE);
	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddress(
			(const GLubyte *) "glXCreateContextAttribsARB");

	//destroy old context
	glXMakeCurrent(g_display, 0, 0);
	glXDestroyContext(g_display, ctx_old);
	if (!glXCreateContextAttribsARB)
	{
		printf("Error: glXCreateContextAttribsARB() not found\n");
		return false;
	}

	//set opengl version
	static int context_attribs[] =
			{
					GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
					GLX_CONTEXT_MINOR_VERSION_ARB, 3,
					None
			};

	//create new context
	g_gc = glXCreateContextAttribsARB(g_display, fbc[0], NULL, true, context_attribs);
	if (!g_gc)
	{
		printf("Error: Failed to create OpenGL Context\n");
		return false;
	}

	//Show window
	XMapWindow(g_display, g_window);
	XStoreName(g_display, g_window, title);

	glXMakeCurrent(g_display, g_window, g_gc);

	int major = 0;
	int minor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	/*
	  printf("OpenGL context created\nVersion %d.%d\nVendor %s\nRenderer %s\n",
	  major, minor,
	  glGetString(GL_VENDOR),
	  glGetString(GL_RENDERER));
	*/
	//glXSwapIntervalEXT(0);
	//int screen_id = DefaultScreen(g_display);
	//const char* glxExts = glXQueryExtensionsString(g_display, screen_id);

	typedef void (*PFNGLXSWAPINTERVALEXTPROC)(Display *dpy, GLXDrawable drawable, int interval);
	PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC) glXGetProcAddress(
			(const GLubyte *) "glXSwapIntervalEXT");

	Display *dpy = glXGetCurrentDisplay();
	GLXDrawable drawable = glXGetCurrentDrawable();
	glXSwapIntervalEXT(dpy, drawable, 0);
	uint swap_min, swap_max;
	if (drawable)
	{
		glXQueryDrawable(g_display, drawable, GLX_SWAP_INTERVAL_EXT, &swap_min);
		glXQueryDrawable(g_display, drawable, GLX_MAX_SWAP_INTERVAL_EXT, &swap_max);
		//printf("The swap interval is %u and the max swap interval is %u\n", swap_min, swap_max);
	}

	g_blank_cursor = create_blank_cursor();

	init_keys();
	return true;
}

void tek_window_swap_buffers()
{
	glXSwapBuffers(g_display, g_window);
}

bool tek_window_update()
{
	XEvent e;
	g_wheel = 0;
	g_num_input_letters = 0;
	while (XPending(g_display))
	{
		XNextEvent(g_display, &e);
		if (XFilterEvent(&e, None))
		{
			continue;
		}

		switch (e.type)
		{
			case ConfigureNotify:
			{
				XConfigureEvent xce = e.xconfigure;

				g_width = xce.width;
				g_height = xce.height;
			}
				break;
			case KeyPress:
			{
				Key mapped_key = get_key_def(e.xkey.keycode);
				g_keys_down[mapped_key] = true;

				TekInputLetter input_letter;

				if (mapped_key == KEY_TAB)
				{
					input_letter.type = INPUT_LETTER_TAB;
				}
				else if (mapped_key == KEY_ENTER)
				{
					input_letter.type = INPUT_LETTER_ENTER;
				}
				else if (mapped_key == KEY_BACKSPACE)
				{
					input_letter.type = INPUT_LETTER_BACKSPACE;
				}
				else if (mapped_key == KEY_ESCAPE)
				{
					input_letter.type = INPUT_LETTER_ESCAPE;
				}
				else if (mapped_key == KEY_SPACE)
				{
					input_letter.type = INPUT_LETTER_SPACE;
				}
				else if (mapped_key == KEY_UP)
				{
					input_letter.type = INPUT_LETTER_UP;
				}
				else if (mapped_key == KEY_DOWN)
				{
					input_letter.type = INPUT_LETTER_DOWN;
				}
				else if (mapped_key == KEY_LEFT)
				{
					input_letter.type = INPUT_LETTER_LEFT;
				}
				else if (mapped_key == KEY_RIGHT)
				{
					input_letter.type = INPUT_LETTER_RIGHT;
				}
				else if (mapped_key == KEY_DELETE)
				{
					input_letter.type = INPUT_LETTER_DELETE;
				}
				else
				{
					input_letter.type = INPUT_LETTER_LETTER;
					input_letter.key = mapped_key;
				}

				if(g_num_input_letters < 8)
				{
					g_input_letters[g_num_input_letters++] = input_letter;
				}
			}
				break;
			case KeyRelease:
			{
				Key mapped_key = get_key_def(e.xkey.keycode);
				g_keys_down[mapped_key] = false;
			}
				break;
			case ButtonPress:
			{
				if (e.xbutton.button == Button4)
				{
					g_wheel = 1;
				}
				else if (e.xbutton.button == Button5)
				{
					g_wheel = -1;
				}
				else
				{
					u32 btn = e.xbutton.button;
					g_buttons_down[btn] = true;
				}
			}
				break;
			case ButtonRelease:
			{
				u32 btn = e.xbutton.button;
				g_buttons_down[btn] = false;
			}
				break;
			case MotionNotify:
			{
				g_mx = e.xmotion.x;
				g_my = e.xmotion.y;
			}
				break;
			case EnterNotify:
			{

			}
				break;
			case LeaveNotify:
			{

			}
				break;
			case ClientMessage:
			{
				//printf("window closed by wm\n");

				g_close_requested = true;

				return false;
			}
			default:
				break;
		}
	}

	return true;
}

bool tek_window_should_close()
{
	return g_close_requested;
}

bool tek_window_is_key_down(Key key)
{
	return g_keys_down[key];
}

bool tek_window_is_mouse_down(MouseButton button)
{
	if (button > 0 && button < NUM_MOUSE_BUTTONS)
		return g_buttons_down[button];
	return false;
}

int tek_window_get_mouse_x()
{
	return g_mx;
}

int tek_window_get_mouse_y()
{
	return g_my;
}

int tek_window_get_mouse_wheel()
{
	return g_wheel;
}

void tek_window_set_cursor_pos(int x, int y)
{
	XWarpPointer(g_display, None, g_window, 0, 0, 0, 0, x, y);
	g_mx = x;
	g_my = y;
}

void tek_window_show_cursor(int val)
{
	if (val)
	{
		XDefineCursor(g_display, g_window, None);
	}
	else
	{
		XDefineCursor(g_display, g_window, g_blank_cursor);
	}
}

TekInputLetter *tek_window_get_letter(int *letters)
{
	*letters = g_num_input_letters;
	return g_input_letters;
}

static Key get_key_def(int key)
{
	if (key < 1 || key > 512)
	{
		return KEY_NONE;
	}

	return g_keys_defs[key];
}

static void init_keys()
{
	for (int i = 0; i < NUM_KEYS; ++i)
	{
		g_keys_down[i] = false;
	}

	for (int i = 0; i < 512; ++i)
	{
		g_keys_defs[i] = KEY_NONE;
	}

	g_keys_defs[9] = KEY_ESCAPE;
	g_keys_defs[67] = KEY_F1;
	g_keys_defs[68] = KEY_F2;
	g_keys_defs[69] = KEY_F3;
	g_keys_defs[70] = KEY_F4;
	g_keys_defs[71] = KEY_F5;
	g_keys_defs[72] = KEY_F6;
	g_keys_defs[73] = KEY_F7;
	g_keys_defs[74] = KEY_F8;
	g_keys_defs[75] = KEY_F9;
	g_keys_defs[76] = KEY_F10;
	g_keys_defs[95] = KEY_F11;
	g_keys_defs[96] = KEY_F12;
	g_keys_defs[78] = KEY_SCROLL_LOCK;
	g_keys_defs[127] = KEY_PAUSE;
	g_keys_defs[10] = KEY_1;
	g_keys_defs[11] = KEY_2;
	g_keys_defs[12] = KEY_3;
	g_keys_defs[13] = KEY_4;
	g_keys_defs[14] = KEY_5;
	g_keys_defs[15] = KEY_6;
	g_keys_defs[16] = KEY_7;
	g_keys_defs[17] = KEY_8;
	g_keys_defs[18] = KEY_9;
	g_keys_defs[19] = KEY_0;
	g_keys_defs[20] = KEY_MINUS;
	g_keys_defs[21] = KEY_EQUAL;
	g_keys_defs[22] = KEY_BACKSPACE;
	g_keys_defs[118] = KEY_INSERT;
	g_keys_defs[110] = KEY_HOME;
	g_keys_defs[112] = KEY_PAGE_UP;
	g_keys_defs[23] = KEY_TAB;
	g_keys_defs[24] = KEY_Q;
	g_keys_defs[25] = KEY_W;
	g_keys_defs[26] = KEY_E;
	g_keys_defs[27] = KEY_R;
	g_keys_defs[28] = KEY_T;
	g_keys_defs[29] = KEY_Y;
	g_keys_defs[30] = KEY_U;
	g_keys_defs[31] = KEY_I;
	g_keys_defs[32] = KEY_O;
	g_keys_defs[33] = KEY_P;
	g_keys_defs[34] = KEY_LEFT_BRACKET;
	g_keys_defs[35] = KEY_RIGHT_BRACKET;
	g_keys_defs[51] = KEY_BACKSLASH;
	g_keys_defs[119] = KEY_DELETE;
	g_keys_defs[115] = KEY_END;
	g_keys_defs[117] = KEY_PAGE_DOWN;
	g_keys_defs[66] = KEY_CAPS_LOCK;
	g_keys_defs[38] = KEY_A;
	g_keys_defs[39] = KEY_S;
	g_keys_defs[40] = KEY_D;
	g_keys_defs[41] = KEY_F;
	g_keys_defs[42] = KEY_G;
	g_keys_defs[43] = KEY_H;
	g_keys_defs[44] = KEY_J;
	g_keys_defs[45] = KEY_K;
	g_keys_defs[46] = KEY_L;
	g_keys_defs[47] = KEY_SEMICOLON;
	g_keys_defs[48] = KEY_APOSTROPHE;
	g_keys_defs[36] = KEY_ENTER;
	g_keys_defs[50] = KEY_LEFT_SHIFT;
	g_keys_defs[52] = KEY_Z;
	g_keys_defs[53] = KEY_X;
	g_keys_defs[54] = KEY_C;
	g_keys_defs[55] = KEY_V;
	g_keys_defs[56] = KEY_B;
	g_keys_defs[57] = KEY_N;
	g_keys_defs[58] = KEY_M;
	g_keys_defs[59] = KEY_COMMA;
	g_keys_defs[60] = KEY_PERIOD;
	g_keys_defs[61] = KEY_SLASH;
	g_keys_defs[62] = KEY_RIGHT_SHIFT;
	g_keys_defs[37] = KEY_RIGHT_CONTROL;
	g_keys_defs[133] = KEY_RIGHT_SUPER;
	g_keys_defs[64] = KEY_RIGHT_ALT;
	g_keys_defs[65] = KEY_SPACE;
	g_keys_defs[108] = KEY_LEFT_ALT;
	g_keys_defs[135] = KEY_MENU;
	g_keys_defs[105] = KEY_RIGHT_CONTROL;
	g_keys_defs[113] = KEY_LEFT;
	g_keys_defs[116] = KEY_DOWN;
	g_keys_defs[114] = KEY_RIGHT;
	g_keys_defs[111] = KEY_UP;
}