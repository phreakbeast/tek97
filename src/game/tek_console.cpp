#include "tek_console.hpp"

#include "../core/tek_core.hpp"
#include "../core/tek_input.hpp"

#include "tek_editor.hpp"

#include <string.h>

typedef struct
{
	char text[256];
	TekColor color;
} ConsoleLine;

static bool g_active = false;
static TekFont g_font;

static ConsoleLine g_history[12];
static u32 g_num_lines = 0;

static ConsoleLine g_curr_line;
static int g_cursor_pos = 0;

static void add_char_to_line(char c)
{
	if (c == 0)
		return;
	size_t len = strlen(g_curr_line.text);
	if (len >= 254)
		return;

	g_curr_line.text[len++] = c;
	g_curr_line.text[len] = '\0';

	g_cursor_pos++;
}

static void insert_char_to_line(char c, u32 pos)
{
	if (c == 0)
		return;
	size_t len = strlen(g_curr_line.text);
	if (len >= 254)
		return;

	char copy[256];
	memcpy(copy, g_curr_line.text, sizeof(char) * 256);
	g_curr_line.text[pos] = c;
	u32 ctr = pos;
	while (copy[ctr] != '\0')
	{
		g_curr_line.text[ctr + 1] = copy[ctr];
		ctr++;
	}
	g_curr_line.text[ctr + 1] = '\0';
	g_cursor_pos++;
}

static void delete_char_from_line(u32 pos)
{
	if (pos == 0)
		return;

	u32 ctr = pos - 1;
	while (g_curr_line.text[ctr] != '\0')
	{
		g_curr_line.text[ctr] = g_curr_line.text[ctr + 1];
		ctr++;
	}
	g_curr_line.text[ctr + 1] = '\0';
	g_cursor_pos--;
}

static void delete_prev_character()
{
	size_t len = strlen(g_curr_line.text);
	if (len == 0)
		return;

	if (g_cursor_pos == len)
	{
		g_curr_line.text[len - 1] = '\0';
		g_cursor_pos--;
	}
	else
	{
		delete_char_from_line(g_cursor_pos);
	}
}

static void delete_next_character()
{
	size_t len = strlen(g_curr_line.text);
	if (g_cursor_pos == len)
	{
		return;
	}
	else
	{
		u32 ctr = g_cursor_pos;
		while (g_curr_line.text[ctr] != '\0')
		{
			g_curr_line.text[ctr] = g_curr_line.text[ctr + 1];
			ctr++;
		}
		g_curr_line.text[ctr + 1] = '\0';
	}
}

static void move_cursor_left()
{
	g_cursor_pos--;
	if (g_cursor_pos <= 0)
		g_cursor_pos = 0;
}

static void move_cursor_right()
{
	g_cursor_pos++;
	size_t len = strlen(g_curr_line.text);
	if (g_cursor_pos > len)
		g_cursor_pos = len;
}

static bool line_begins_with(const char *text)
{
	bool res = true;
	size_t len = strlen(text);
	for (u32 i = 0; i < len; ++i)
	{
		if (g_curr_line.text[i] != text[i])
		{
			res = false;
			break;
		}
	}
	return res;
}

static int process_line()
{
	if (line_begins_with("save map"))
	{
		char name[64];
		int res = sscanf(g_curr_line.text, "save map %s", name);
		if (tek_editor_save_map(name))
			return 1;
		else
			return 2;
	}
	else if (line_begins_with("load map"))
	{
		char name[64];
		int res = sscanf(g_curr_line.text, "load map %s", name);
		if(tek_editor_load_map(name))
			return 1;
		else
			return 2;
	}
	return 0;
}

static void confirm_line()
{
	size_t len = strlen(g_curr_line.text);
	if (len == 0)
		return;

	int res = process_line();

	if (g_num_lines < 12)
	{
		g_history[g_num_lines++] = g_curr_line;
		g_curr_line.text[0] = '\0';
	}
	else
	{
		for (u32 i = 0; i < 11; ++i)
		{
			memcpy(&g_history[i], &g_history[i + 1], sizeof(ConsoleLine));
		}
		g_history[g_num_lines - 1] = g_curr_line;
		g_curr_line.text[0] = '\0';
	}

	if (res == 1)
	{
		g_history[g_num_lines - 1].color = tek_color_green();
	}
	else if (res == 2)
	{
		g_history[g_num_lines - 1].color = tek_color_red();
	}

	g_cursor_pos = 0;
}

bool tek_console_init()
{
	if(!tek_font_load(&g_font, "data/fonts/test.font"))
		return false;

	g_curr_line.text[0] = '\0';
	g_curr_line.color = tek_color_white();

	return true;
}

void tek_console_destroy()
{
	tek_font_destroy(&g_font);
}

void tek_console_update(float delta)
{
	if (g_active)
	{
		TekKeyboardState *kstate = tek_input_get_key_state();
		for (u32 i = 0; i < kstate->num_letters; ++i)
		{
			TekInputLetter letter = kstate->letters[i];
			if (letter.type == INPUT_LETTER_LETTER)
			{
				bool shift = false;
				if (kstate->keys_down[KEY_LEFT_SHIFT] || kstate->keys_down[KEY_RIGHT_SHIFT])
					shift = true;
				char c = tek_printable_char(letter.key, shift);

				size_t len = strlen(g_curr_line.text);
				if (g_cursor_pos == len)
				{
					add_char_to_line(c);
				}
				else
				{
					insert_char_to_line(c, g_cursor_pos);
				}
			}
			else if (letter.type == INPUT_LETTER_ENTER)
			{
				confirm_line();
			}
			else if (letter.type == INPUT_LETTER_SPACE)
			{
				size_t len = strlen(g_curr_line.text);
				if (g_cursor_pos == len)
				{
					add_char_to_line(' ');
				}
				else
				{
					insert_char_to_line(' ', g_cursor_pos);
				}
			}
			else if (letter.type == INPUT_LETTER_BACKSPACE)
			{
				delete_prev_character();
			}
			else if (letter.type == INPUT_LETTER_DELETE)
			{
				delete_next_character();
			}
			else if (letter.type == INPUT_LETTER_LEFT)
			{
				move_cursor_left();
			}
			else if (letter.type == INPUT_LETTER_RIGHT)
			{
				move_cursor_right();
			}
		}
	}
}

void tek_console_render(TekSpritebatch *sb)
{
	if (g_active)
	{
		tek_sb_render_rect(sb, tek_rect_create(0, 0, 640, 100), tek_color_create(128, 128, 128));
		tek_sb_render_rect(sb, tek_rect_create(0, 100, 640, 16), tek_color_create(100, 100, 100));

		float cur_pos = g_cursor_pos * 8;
		tek_sb_render_rect(sb, tek_rect_create(cur_pos, 104, 8, 8), tek_color_create(180, 180, 180));

		tek_sb_render_text(sb, g_curr_line.text, &g_font, 0, 104, g_curr_line.color, 640);

		for (u32 i = 0; i < 12; ++i)
		{
			float pos_y = i * 8.0f;
			tek_sb_render_text(sb, g_history[i].text, &g_font, 0, pos_y, g_history[i].color, 640);
		}
	}
}

void tek_console_toggle()
{
	g_active = !g_active;
}

bool tek_console_active()
{
	return g_active;
}