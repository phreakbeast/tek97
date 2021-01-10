#ifndef _TEK_IWINDOW_HPP
#define _TEK_IWINDOW_HPP

#include "../core/tek_core.hpp"

namespace tek
{
	class Application;

	class IWindow
	{
	public:
		IWindow(){application = nullptr;}
		virtual ~IWindow(){};

		virtual bool open(u32 width, u32 height, const char *title, bool fullscreen)=0;

		virtual void toggle_fullscreen()=0;

		virtual void swap_buffers()=0;

		virtual bool update()=0;

		virtual bool should_close()=0;

		virtual bool is_key_down(Key key)=0;

		virtual bool is_mouse_down(MouseButton button)=0;

		virtual int get_mouse_x()=0;

		virtual int get_mouse_y()=0;

		virtual int get_mouse_wheel()=0;

		virtual void set_cursor_pos(int x, int y)=0;

		virtual void show_cursor(int val)=0;

		void set_app_object(Application* application){this->application = application;}
	protected:
		Application* application;
	};
}

#endif