#ifndef _TEK_LINUX_WINDOW_HPP
#define _TEK_LINUX_WINDOW_HPP

#include "../tek_iwindow.hpp"
#include "../tek_platform.hpp"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

namespace tek
{
	class LinuxWindow : public IWindow
	{
	public:
		LinuxWindow();
		~LinuxWindow();

		bool open(u32 width, u32 height, const char *title, bool fullscreen) override;

		void toggle_fullscreen() override;

		void swap_buffers() override;

		bool update() override;

		bool should_close() override;

		bool is_key_down(Key key) override;

		bool is_mouse_down(MouseButton button) override;

		int get_mouse_x() override;

		int get_mouse_y() override;

		int get_mouse_wheel() override;

		void set_cursor_pos(int x, int y) override;

		void show_cursor(int val) override;

	private:
		void init_keys();
		Key get_key_def(int key);
		Cursor create_blank_cursor();

		u32 width;
		u32 height;

		typedef GLXContext(*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig, GLXContext, Bool, const int *);

		Display *display;
		Window window;
		GLXContext gc;

		Window root;
		XVisualInfo *vi;
		Colormap cmap;
		XSetWindowAttributes swa;
		bool keys_down[NUM_KEYS];
		bool buttons_down[NUM_MOUSE_BUTTONS];

		Cursor blank_cursor;

		int mx;
		int my;
		int wheel;

		bool close_requested;

		Key keys_defs[512];
	};
}

#endif