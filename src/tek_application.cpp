#include "tek_application.hpp"

#include "drawing/tek_drawing.hpp"
#include "platform/tek_platform.hpp"
#include "platform/tek_input.hpp"
#include "platform/linux/tek_linux_window.hpp"

namespace tek
{
	Application::Application()
	{
		window = nullptr;
		input = nullptr;
		renderer = nullptr;

		is_running = true;

		update_timer = 0;
		update_ticks = 1.0f / 60.0f;
		timer = 0;

		frames = 0;
		updates = 0;
		fps = 0;
		ups = 0;

		delta = 0;

		width = 0;
		height = 0;
		render_width = 0;
		render_height = 0;
	}

	Application::~Application()
	{
		delete fb;
		delete renderer;
		delete input;
		delete window;
	}

	bool Application::init(u32 width, u32 height, u32 render_width, u32 render_height, const char *title)
	{
		this->width = width;
		this->height = height;
		this->render_width = render_width;
		this->render_height = render_height;

		window = new LinuxWindow();
		if (!window->open(width, height, title, 0))
			return false;
		window->set_app_object(this);

		input = new Input(window);
		if (!input->init())
			return false;

		renderer = new Renderer();
		if (!renderer->init(render_width, render_height))
			return false;

		tek_time_init();

		target_rect = get_target_rect(width,height,render_width,render_height);
		ortho = mat4_ortho(0,width,0,height,-1,1);
		ortho = mat4_transposed(&ortho);

		fb = new TekFramebuffer(render_width,render_height);

		return on_init();
	}

	void Application::start_main_loop()
	{
		ups_timer.start();
		delta_timer.start();

		while (is_running)
		{
			tek::TekRenderStats *stats = renderer->get_stats();
			double seconds = ups_timer.get_seconds();
			if (seconds - update_timer > update_ticks)
			{
				delta = (float)delta_timer.get_seconds();
				delta_timer.start();
				if (!window->update())
				{
					break;
				}
				input->update();

				tek::TekKeyboardState *kstate = input->get_key_state();
				if (kstate->keys_pressed[KEY_ESCAPE])
				{
					break;
				}

				is_running = !window->should_close();

				on_update(delta);

				update_timer += update_ticks;
				updates++;
			}

			renderer->start_frame();
			renderer->bind_framebuffer(fb, tek::TekColor::black());

			on_render();

			renderer->unbind_framebuffer();
			renderer->viewport(width, height);
			fb->bind_reading(0);
			renderer->render_sprite(target_rect, tek::TekRect(0, 0, 1, 1), fb->tex_id, &ortho);

			window->swap_buffers();
			frames++;

			if (seconds - timer > 1.0f)
			{
				timer += 1.0f;
				//tick;

				fps = frames;
				ups = updates;
				frames = 0;
				updates = 0;
				stats->fps = fps;
				stats->ups = ups;
				stats->delta = delta;
			}
		}
	}

	void Application::resize(u32 width, u32 height)
	{
		this->width = width;
		this->height = height;
		target_rect = get_target_rect(width,height,render_width,render_height);
		ortho = mat4_ortho(0,width,0,height,-1,1);
		ortho = mat4_transposed(&ortho);
	}

	TekRect Application::get_target_rect(u32 win_width, u32 win_height, u32 render_width, u32 render_height)
	{
		TekRect result;

		if (win_width == render_width && win_height == render_height)
		{
			float x, y, w, h;
			x = 0;
			y = 0;
			w = (float) win_width;
			h = (float) win_height;
			result = tek::TekRect(x, y, w, h);
			return result;
		}

		float ar = (float) win_width / win_height;
		float st_ar = (float) render_width / render_height;

		// screen wider than 16:9
		if (ar > st_ar)
		{
			float new_width = win_height * st_ar;
			float x, y, w, h;

			x = (win_width - new_width) / 2;
			w = new_width;
			y = 0;
			h = (float) win_height;
			result = tek::TekRect(x, y, w, h);
		}
		else
		{
			float x, y, w, h;
			float new_height = win_width / st_ar;
			x = 0;
			w = (float) win_width;

			y = (win_height - new_height) / 2;
			h = new_height;
			result = tek::TekRect(x, y, w, h);
		}

		return result;
	}
}