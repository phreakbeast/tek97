#include "tek_linux_window.hpp"

#include <stdio.h>
#include <assert.h>

#include "../../tek_application.hpp"

namespace tek
{
	Cursor LinuxWindow::create_blank_cursor()
	{
		static char data[1] =
				{0};
		Cursor cursor;
		Pixmap blank;
		XColor dummy;

		blank = XCreateBitmapFromData(display, root, data, 1, 1);
		if (blank == None)
		{
			printf("Fatal Error: not enough memory for cursor\n");
		}
		cursor = XCreatePixmapCursor(display, blank, blank, &dummy, &dummy, 0, 0);
		XFreePixmap(display, blank);

		return cursor;
	}

	LinuxWindow::LinuxWindow()
	{
		width = 0;
		height = 0;
		mx = 0;
		my = 0;
		wheel = 0;
		close_requested = false;
	}

	LinuxWindow::~LinuxWindow()
	{
		XFreeCursor(display, blank_cursor);
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

	bool LinuxWindow::open(u32 width, u32 height, const char *title, bool fullscreen)
	{
		printf("creating window ...\n");

		this->width = width;
		this->height = height;

		display = XOpenDisplay(NULL);
		assert(display);

		int number_of_screens = XScreenCount(display);
		printf("number of screens %d\n", number_of_screens);

		root = DefaultRootWindow(display);

		GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};

		vi = glXChooseVisual(display, 0, att);
		assert(vi);

		cmap = XCreateColormap(display, root, vi->visual, AllocNone);

		swa.colormap = cmap;
		swa.event_mask =
				StructureNotifyMask | ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask |
				ButtonReleaseMask |
				PointerMotionMask | EnterWindowMask | LeaveWindowMask;

		window = XCreateWindow(display, root, 0, 0, width, height, 0,
		                         vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

		//window manager should be able to close the window
		Atom del_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
		XSetWMProtocols(display, window, &del_window, 1);

		//for tiled window managers
		const char *wintype_name = "_NET_WM_WINDOW_TYPE_UTILITY";
		Atom _NET_WM_WINDOW_TYPE = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
		Atom wintype = XInternAtom(display, wintype_name, False);
		XChangeProperty(display, window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32, PropModeReplace,
		                (unsigned char *) &wintype,
		                1);

		long compositor = 2;
		Atom _NET_WM_BYPASS_COMPOSITOR = XInternAtom(display, "_NET_WM_BYPASS_COMPOSITOR", False);
		XChangeProperty(display, window, _NET_WM_BYPASS_COMPOSITOR, XA_CARDINAL, 32, PropModeReplace,
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
		GLXFBConfig *fbc = glXChooseFBConfig(display, DefaultScreen(display), visual_attribs, &num_fbc);
		if (!fbc)
		{
			printf("Error: glXChoosevisual() failed\n");
			return false;
		}

		//create old OpenGL context
		XVisualInfo *vi = glXGetVisualFromFBConfig(display, fbc[0]);
		GLXContext ctx_old = glXCreateContext(display, vi, 0, GL_TRUE);
		glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
		glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddress(
				(const GLubyte *) "glXCreateContextAttribsARB");

		//destroy old context
		glXMakeCurrent(display, 0, 0);
		glXDestroyContext(display, ctx_old);
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
		gc = glXCreateContextAttribsARB(display, fbc[0], NULL, true, context_attribs);
		if (!gc)
		{
			printf("Error: Failed to create OpenGL Context\n");
			return false;
		}

		//Show window
		XMapWindow(display, window);
		XStoreName(display, window, title);

		glXMakeCurrent(display, window, gc);

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
			glXQueryDrawable(display, drawable, GLX_SWAP_INTERVAL_EXT, &swap_min);
			glXQueryDrawable(display, drawable, GLX_MAX_SWAP_INTERVAL_EXT, &swap_max);
			//printf("The swap interval is %u and the max swap interval is %u\n", swap_min, swap_max);
		}

		blank_cursor = create_blank_cursor();

		init_keys();
		return true;
	}

	void LinuxWindow::swap_buffers()
	{
		glXSwapBuffers(display, window);
	}

	bool LinuxWindow::update()
	{
		XEvent e;
		wheel = 0;
		while (XPending(display))
		{
			XNextEvent(display, &e);
			if (XFilterEvent(&e, None))
			{
				continue;
			}

			switch (e.type)
			{
				case ConfigureNotify:
				{
					XConfigureEvent xce = e.xconfigure;

					width = xce.width;
					height = xce.height;
					if(application)
					{
						application->resize(width,height);
					}
				}
					break;
				case KeyPress:
				{
					Key mapped_key = get_key_def(e.xkey.keycode);
					keys_down[mapped_key] = true;

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
				}
					break;
				case KeyRelease:
				{
					Key mapped_key = get_key_def(e.xkey.keycode);
					keys_down[mapped_key] = false;
				}
					break;
				case ButtonPress:
				{
					if (e.xbutton.button == Button4)
					{
						wheel = 1;
					}
					else if (e.xbutton.button == Button5)
					{
						wheel = -1;
					}
					else
					{
						u32 btn = e.xbutton.button;
						buttons_down[btn] = true;
					}
				}
					break;
				case ButtonRelease:
				{
					u32 btn = e.xbutton.button;
					buttons_down[btn] = false;
				}
					break;
				case MotionNotify:
				{
					mx = e.xmotion.x;
					my = e.xmotion.y;
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

					close_requested = true;

					return false;
				}
				default: break;
			}
		}

		return true;
	}

	bool LinuxWindow::should_close()
	{
		return close_requested;
	}

	bool LinuxWindow::is_key_down(Key key)
	{
		return keys_down[key];
	}

	bool LinuxWindow::is_mouse_down(MouseButton button)
	{
		if (button > 0 && button < NUM_MOUSE_BUTTONS)
			return buttons_down[button];
		return false;
	}

	int LinuxWindow::get_mouse_x()
	{
		return mx;
	}

	int LinuxWindow::get_mouse_y()
	{
		return my;
	}

	int LinuxWindow::get_mouse_wheel()
	{
		return wheel;
	}

	void LinuxWindow::set_cursor_pos(int x, int y)
	{
		XWarpPointer(display, None, window, 0, 0, 0, 0, x, y);
		mx = x;
		my = y;
	}

	void LinuxWindow::show_cursor(int val)
	{
		if (val)
		{
			XDefineCursor(display, window, None);
		}
		else
		{
			XDefineCursor(display, window, blank_cursor);
		}
	}

	void LinuxWindow::toggle_fullscreen()
	{

	}

	Key LinuxWindow::get_key_def(int key)
	{
		if (key < 1 || key > 512)
		{
			return KEY_NONE;
		}

		return keys_defs[key];
	}

	void LinuxWindow::init_keys()
	{
		for (int i = 0; i < NUM_KEYS; ++i)
		{
			keys_down[i] = false;
		}

		for (int i = 0; i < 512; ++i)
		{
			keys_defs[i] = KEY_NONE;
		}

		keys_defs[9] = KEY_ESCAPE;
		keys_defs[67] = KEY_F1;
		keys_defs[68] = KEY_F2;
		keys_defs[69] = KEY_F3;
		keys_defs[70] = KEY_F4;
		keys_defs[71] = KEY_F5;
		keys_defs[72] = KEY_F6;
		keys_defs[73] = KEY_F7;
		keys_defs[74] = KEY_F8;
		keys_defs[75] = KEY_F9;
		keys_defs[76] = KEY_F10;
		keys_defs[95] = KEY_F11;
		keys_defs[96] = KEY_F12;
		keys_defs[78] = KEY_SCROLL_LOCK;
		keys_defs[127] = KEY_PAUSE;
		keys_defs[10] = KEY_1;
		keys_defs[11] = KEY_2;
		keys_defs[12] = KEY_3;
		keys_defs[13] = KEY_4;
		keys_defs[14] = KEY_5;
		keys_defs[15] = KEY_6;
		keys_defs[16] = KEY_7;
		keys_defs[17] = KEY_8;
		keys_defs[18] = KEY_9;
		keys_defs[19] = KEY_0;
		keys_defs[20] = KEY_MINUS;
		keys_defs[21] = KEY_EQUAL;
		keys_defs[22] = KEY_BACKSPACE;
		keys_defs[118] = KEY_INSERT;
		keys_defs[110] = KEY_HOME;
		keys_defs[112] = KEY_PAGE_UP;
		keys_defs[23] = KEY_TAB;
		keys_defs[24] = KEY_Q;
		keys_defs[25] = KEY_W;
		keys_defs[26] = KEY_E;
		keys_defs[27] = KEY_R;
		keys_defs[28] = KEY_T;
		keys_defs[29] = KEY_Y;
		keys_defs[30] = KEY_U;
		keys_defs[31] = KEY_I;
		keys_defs[32] = KEY_O;
		keys_defs[33] = KEY_P;
		keys_defs[34] = KEY_LEFT_BRACKET;
		keys_defs[35] = KEY_RIGHT_BRACKET;
		keys_defs[51] = KEY_BACKSLASH;
		keys_defs[119] = KEY_DELETE;
		keys_defs[115] = KEY_END;
		keys_defs[117] = KEY_PAGE_DOWN;
		keys_defs[66] = KEY_CAPS_LOCK;
		keys_defs[38] = KEY_A;
		keys_defs[39] = KEY_S;
		keys_defs[40] = KEY_D;
		keys_defs[41] = KEY_F;
		keys_defs[42] = KEY_G;
		keys_defs[43] = KEY_H;
		keys_defs[44] = KEY_J;
		keys_defs[45] = KEY_K;
		keys_defs[46] = KEY_L;
		keys_defs[47] = KEY_SEMICOLON;
		keys_defs[48] = KEY_APOSTROPHE;
		keys_defs[36] = KEY_ENTER;
		keys_defs[50] = KEY_LEFT_SHIFT;
		keys_defs[52] = KEY_Z;
		keys_defs[53] = KEY_X;
		keys_defs[54] = KEY_C;
		keys_defs[55] = KEY_V;
		keys_defs[56] = KEY_B;
		keys_defs[57] = KEY_N;
		keys_defs[58] = KEY_M;
		keys_defs[59] = KEY_COMMA;
		keys_defs[60] = KEY_PERIOD;
		keys_defs[61] = KEY_SLASH;
		keys_defs[62] = KEY_RIGHT_SHIFT;
		keys_defs[37] = KEY_RIGHT_CONTROL;
		keys_defs[133] = KEY_RIGHT_SUPER;
		keys_defs[64] = KEY_RIGHT_ALT;
		keys_defs[65] = KEY_SPACE;
		keys_defs[108] = KEY_LEFT_ALT;
		keys_defs[135] = KEY_MENU;
		keys_defs[105] = KEY_RIGHT_CONTROL;
		keys_defs[113] = KEY_LEFT;
		keys_defs[116] = KEY_DOWN;
		keys_defs[114] = KEY_RIGHT;
		keys_defs[111] = KEY_UP;
	}
}