#ifndef _TEK_APPLICATION_HPP
#define _TEK_APPLICATION_HPP

#include "core/tek_core.hpp"
#include "drawing/tek_rect.hpp"
#include "drawing/tek_framebuffer.hpp"

#include "drawing/tek_drawing.hpp"
#include "platform/tek_iwindow.hpp"
#include "platform/tek_input.hpp"

namespace tek
{
	class Application
	{
	public:
		Application();
		~Application();

		bool init(u32 width, u32 height, u32 render_width, u32 render_height, const char *title);

		void start_main_loop();

		virtual bool on_init() = 0;
		virtual void on_render() = 0;
		virtual void on_update(float delta) = 0;
		virtual void on_resize(u32 width, u32 height) = 0;
		void resize(u32 width, u32 height);
	protected:
		TekRect get_target_rect(u32 win_width, u32 win_height, u32 render_width, u32 render_height);
		IWindow* window;
		Input* input;
		Renderer* renderer;

		bool is_running;

		float update_timer;
		float update_ticks;
		float timer;

		u32 frames;
		u32 updates;
		u32 fps;
		u32 ups;

		float delta;

		tek::TekStopwatch ups_timer;
		tek::TekStopwatch delta_timer;

		u32 width;
		u32 height;
		u32 render_width;
		u32 render_height;

		TekRect target_rect;
		TekFramebuffer *fb;
		Mat4 ortho;
	};
}
#endif
