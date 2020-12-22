#include "tek_editor.hpp"

#include "../math/tek_transform.hpp"
#include "tek_map.hpp"
#include "../drawing/tek_meshbuffer.hpp"
#include "../drawing/tek_shapebuffer.hpp"
#include "../core/tek_input.hpp"

static TekMap g_map;
static TekCamera *g_camera;

static TekLightManager g_lights;
static TekMeshbuffer *g_buffer;
static TekShapebuffer g_shapebuffer;

//editor variables
enum EditorMode
{
	EDITOR_MODE_BRUSH,
	EDITOR_MODE_QUAD,
	EDITOR_MODE_OBJECT,
	EDITOR_MODE_LIGHT,
	NUM_EDITOR_MODES
};

enum BrushEditMode
{
	BRUSH_EDIT_BRUSH,
	BRUSH_EDIT_PLANE,
	BRUSH_EDIT_LINE,
	BRUSH_EDIT_POINT
};

struct EditorState
{
	int selected_object;
	int selected_plane;
	int selected_line;
	int selected_point;
};

static EditorState g_states[NUM_EDITOR_MODES];
static EditorMode g_editor_mode = EDITOR_MODE_BRUSH;
static BrushEditMode g_brush_edit_mode = BRUSH_EDIT_BRUSH;

static void brush_edit_mode_handle_key(Key key);
static void quad_edit_mode_handle_key(Key key);

bool tek_editor_init(u32 width, u32 height)
{
	g_states[0].selected_object = 0;
	g_states[0].selected_plane = 0;
	g_states[0].selected_line = 0;
	g_states[0].selected_point = 0;

	g_states[1].selected_object = 0;
	g_states[1].selected_plane = 0;
	g_states[1].selected_line = 0;
	g_states[1].selected_point = 0;

	g_states[2].selected_object = 0;
	g_states[2].selected_plane = 0;
	g_states[2].selected_line = 0;
	g_states[2].selected_point = 0;

	g_states[3].selected_object = 0;
	g_states[3].selected_plane = 0;
	g_states[3].selected_line = 0;
	g_states[3].selected_point = 0;

	Mat4 projection = mat4_perspective(45.0f, (float) width / height, 0.1f, 100.0f);
	g_camera = new TekCamera(TekCamera::Type::Third, projection);

	g_lights.use_dlight = true;
	g_lights.dlight.color = TekColor(10, 10, 10).to_vec3();
	g_lights.dlight.direction = vec3_create(0, -0.75f, -0.25f);
	g_lights.dlight.intensity = 0.1f;

	g_lights.num_plights = 2;
	g_lights.plights[0].color = TekColor::white().to_vec3();
	g_lights.plights[0].position = vec3_create(-1, 1, 0);
	g_lights.plights[0].range = 3.0f;

	g_lights.plights[1].color = TekColor::blue().to_vec3();
	g_lights.plights[1].position = vec3_create(1, 1, 0);
	g_lights.plights[1].range = 2.0f;

	g_buffer = new TekMeshbuffer(g_camera, &g_lights);
	tek_shapebuffer_init(&g_shapebuffer, g_camera);

	tek_map_init(&g_map);

	return true;
}

void tek_editor_destroy()
{
	tek_map_destroy(&g_map);
	delete g_buffer;
	tek_shapebuffer_destroy(&g_shapebuffer);
}

void tek_editor_update(float delta)
{
	TekKeyboardState *kstate = tek_input_get_key_state();

	if (kstate->keys_down[KEY_W])
	{
		g_camera->move(0, delta * 10);
	}

	if (kstate->keys_down[KEY_S])
	{
		g_camera->move(180, delta * 10);
	}

	if (kstate->keys_down[KEY_A])
	{
		g_camera->move(90, delta * 10);
	}

	if (kstate->keys_down[KEY_D])
	{
		g_camera->move(270, delta * 10);
	}

	if (kstate->keys_down[KEY_Q])
	{
		g_camera->rotate_y(delta * 50);
	}

	if (kstate->keys_down[KEY_E])
	{
		g_camera->rotate_y(-delta * 50);
	}

	if (kstate->keys_pressed[KEY_LEFT])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_LEFT);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_LEFT);
		}
	}
	if (kstate->keys_pressed[KEY_RIGHT])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_RIGHT);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_RIGHT);
		}
	}
	if (kstate->keys_pressed[KEY_UP])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_UP);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_UP);
		}
	}
	if (kstate->keys_pressed[KEY_DOWN])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_DOWN);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_DOWN);
		}
	}
	if (kstate->keys_pressed[KEY_PAGE_UP])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_PAGE_UP);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_PAGE_UP);
		}
	}
	if (kstate->keys_pressed[KEY_PAGE_DOWN])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_PAGE_DOWN);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_PAGE_DOWN);
		}
	}

	if (kstate->keys_pressed[KEY_1])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_1);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_1);
		}
	}

	if (kstate->keys_pressed[KEY_2])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_2);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_2);
		}
	}

	if (kstate->keys_pressed[KEY_3])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_3);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_3);
		}
	}

	if (kstate->keys_pressed[KEY_4])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_4);
		}
	}

	if (kstate->keys_pressed[KEY_N])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_N);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_N);
		}
	}

	if (kstate->keys_pressed[KEY_P])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_P);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_P);
		}
	}
	if (kstate->keys_pressed[KEY_B])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_B);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_B);
		}
	}
	if (kstate->keys_pressed[KEY_LEFT_BRACKET])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_LEFT_BRACKET);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_LEFT_BRACKET);
		}
	}
	if (kstate->keys_pressed[KEY_RIGHT_BRACKET])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_RIGHT_BRACKET);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_RIGHT_BRACKET);
		}
	}
	if (kstate->keys_pressed[KEY_MINUS])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_MINUS);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_MINUS);
		}
	}
	if (kstate->keys_pressed[KEY_EQUAL])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_EQUAL);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_EQUAL);
		}
	}
	if (kstate->keys_pressed[KEY_I])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_I);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_I);
		}
	}
	if (kstate->keys_pressed[KEY_K])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_K);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_K);
		}
	}
	if (kstate->keys_pressed[KEY_J])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_J);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_J);
		}
	}
	if (kstate->keys_pressed[KEY_L])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_L);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_L);
		}
	}
	if (kstate->keys_pressed[KEY_R])
	{
		if (g_editor_mode == EDITOR_MODE_BRUSH)
		{
			brush_edit_mode_handle_key(KEY_R);
		}
		else if (g_editor_mode == EDITOR_MODE_QUAD)
		{
			quad_edit_mode_handle_key(KEY_R);
		}
	}

	if (kstate->keys_pressed[KEY_F1])
	{
		g_editor_mode = EDITOR_MODE_BRUSH;
	}
	if (kstate->keys_pressed[KEY_F2])
	{
		g_editor_mode = EDITOR_MODE_QUAD;
	}
	if (kstate->keys_pressed[KEY_F3])
	{
		g_editor_mode = EDITOR_MODE_OBJECT;
	}
	if (kstate->keys_pressed[KEY_F4])
	{
		g_editor_mode = EDITOR_MODE_LIGHT;
	}
	g_camera->calc();
}

void tek_editor_render_3d()
{
	//render 3d
	g_buffer->reset();
	tek_shapebuffer_reset(&g_shapebuffer);
	//tek_renderer_draw_mesh(&g_plane, &g_material, &g_transform2.matrix, &g_camera, nullptr,nullptr,0);
	//tek_renderer_draw_mesh(&g_mesh, &g_material, &g_transform.matrix, &g_camera, &g_sun,g_lights,2);

	tek_map_render(&g_map, g_buffer);

	//debug draw
	//draw selected brush
	int mode = 0;
	int selected = 0;
	switch (g_editor_mode)
	{
		case EDITOR_MODE_BRUSH:
		{
			if (g_brush_edit_mode == BRUSH_EDIT_BRUSH)
			{
				mode = 1;
				selected = g_states[EDITOR_MODE_BRUSH].selected_object;
			}
			else if (g_brush_edit_mode == BRUSH_EDIT_PLANE)
			{
				mode = 2;
				selected = g_states[EDITOR_MODE_BRUSH].selected_plane;
			}
			else if (g_brush_edit_mode == BRUSH_EDIT_LINE)
			{
				mode = 3;
				selected = g_states[EDITOR_MODE_BRUSH].selected_line;
			}
			else if (g_brush_edit_mode == BRUSH_EDIT_POINT)
			{
				mode = 4;
				selected = g_states[EDITOR_MODE_BRUSH].selected_point;
			}

			int selected_brush = g_states[EDITOR_MODE_BRUSH].selected_object;
			if (selected_brush >= 0)
			{
				g_map.brushes[selected_brush].render_shape(&g_shapebuffer, mode, selected);
			}
		}break;
		case EDITOR_MODE_QUAD:
		{
			if (g_brush_edit_mode == BRUSH_EDIT_BRUSH)
			{
				mode = 1;
				selected = g_states[EDITOR_MODE_QUAD].selected_object;
			}
			else if (g_brush_edit_mode == BRUSH_EDIT_LINE)
			{
				mode = 2;
				selected = g_states[EDITOR_MODE_QUAD].selected_line;
			}
			else if (g_brush_edit_mode == BRUSH_EDIT_POINT)
			{
				mode = 3;
				selected = g_states[EDITOR_MODE_QUAD].selected_point;
			}

			int selected_quad = g_states[EDITOR_MODE_QUAD].selected_object;
			if (selected_quad >= 0)
			{
				tek_quad_render_shape(&g_map.quads[selected_quad],&g_shapebuffer,mode,selected);
			}
		}break;
		default:
			break;
	}


	//tek_shapebuffer_draw_line(&g_shapebuffer, vec3_create(0, 0, 0), vec3_create(0, 3, 0), TekColor::green());
	//g_buffer->draw_mesh(&g_plane, &g_material, &g_transform2);
	//g_buffer->draw_mesh(&g_mesh, &g_material, &g_transform);

	tek_shapebuffer_render(&g_shapebuffer);
	g_buffer->render();
}

void tek_editor_render_2d(TekSpritebatch* sb, TekFont* font)
{
	char mode_str[256];

	sprintf(mode_str, "editor_mode: %d edit_mode: %d", g_editor_mode, g_brush_edit_mode);

	tek_sb_render_text(sb, mode_str, font, 400, 5, TekColor::white(), 0);
}

static void brush_edit_mode_move(Vec3 vec)
{
	int selected_brush = g_states[EDITOR_MODE_BRUSH].selected_object;
	if (g_brush_edit_mode == BRUSH_EDIT_BRUSH)
	{
		g_map.brushes[selected_brush].move(vec.x, vec.y, vec.z);
	}
	else if (g_brush_edit_mode == BRUSH_EDIT_PLANE)
	{
		int selected_plane = g_states[EDITOR_MODE_BRUSH].selected_plane;
		g_map.brushes[selected_brush].move_plane(selected_plane, vec.x, vec.y, vec.z);
	}
	else if (g_brush_edit_mode == BRUSH_EDIT_LINE)
	{
		int selected_line = g_states[EDITOR_MODE_BRUSH].selected_line;
		g_map.brushes[selected_brush].move_line(selected_line, vec.x, vec.y, vec.z);
	}
	else if (g_brush_edit_mode == BRUSH_EDIT_POINT)
	{
		int selected_point = g_states[EDITOR_MODE_BRUSH].selected_point;
		g_map.brushes[selected_brush].move_point(selected_point, vec.x, vec.y, vec.z);
	}
}

static void brush_edit_select_next()
{
	if (g_brush_edit_mode == BRUSH_EDIT_BRUSH)
	{
		int selected_brush = g_states[EDITOR_MODE_BRUSH].selected_object;
		selected_brush++;
		if (selected_brush >= g_map.brushes.size())
			selected_brush = 0;
		g_states[EDITOR_MODE_BRUSH].selected_object = selected_brush;
	}
	else if (g_brush_edit_mode == BRUSH_EDIT_PLANE)
	{
		int selected_plane = g_states[EDITOR_MODE_BRUSH].selected_plane;
		selected_plane++;
		if (selected_plane > 5)
			selected_plane = 0;
		g_states[EDITOR_MODE_BRUSH].selected_plane = selected_plane;
	}
	else if (g_brush_edit_mode == BRUSH_EDIT_LINE)
	{
		int selected_line = g_states[EDITOR_MODE_BRUSH].selected_line;
		selected_line++;
		if (selected_line > 11)
			selected_line = 0;
		g_states[EDITOR_MODE_BRUSH].selected_line = selected_line;
	}
	else if (g_brush_edit_mode == BRUSH_EDIT_POINT)
	{
		int selected_point = g_states[EDITOR_MODE_BRUSH].selected_point;
		selected_point++;
		if (selected_point > 7)
			selected_point = 0;
		g_states[EDITOR_MODE_BRUSH].selected_point = selected_point;
	}
}

static void brush_edit_select_prev()
{
	if (g_brush_edit_mode == BRUSH_EDIT_BRUSH)
	{
		int selected_brush = g_states[EDITOR_MODE_BRUSH].selected_object;
		selected_brush--;
		if (selected_brush < 0)
		{
			selected_brush = g_map.brushes.size() - 1;
		}
		g_states[EDITOR_MODE_BRUSH].selected_object = selected_brush;
	}
	else if (g_brush_edit_mode == BRUSH_EDIT_PLANE)
	{
		int selected_plane = g_states[EDITOR_MODE_BRUSH].selected_plane;
		selected_plane--;
		if (selected_plane < 0)
			selected_plane = 5;
		g_states[EDITOR_MODE_BRUSH].selected_plane = selected_plane;
	}
	else if (g_brush_edit_mode == BRUSH_EDIT_LINE)
	{
		int selected_line = g_states[EDITOR_MODE_BRUSH].selected_line;
		selected_line--;
		if (selected_line < 0)
			selected_line = 11;
		g_states[EDITOR_MODE_BRUSH].selected_line = selected_line;
	}
	else if (g_brush_edit_mode == BRUSH_EDIT_POINT)
	{
		int selected_point = g_states[EDITOR_MODE_BRUSH].selected_point;
		selected_point--;
		if (selected_point < 0)
			selected_point = 7;
		g_states[EDITOR_MODE_BRUSH].selected_point = selected_point;
	}
}

static void brush_edit_mode_add_brush()
{
	g_map.brushes.push_back(TekBrush());
	g_states[EDITOR_MODE_BRUSH].selected_object = g_map.brushes.size() - 1;
	int selected_brush = g_states[EDITOR_MODE_BRUSH].selected_object;
	g_map.brushes[selected_brush].set_material(&g_map.materials[0].material, 0);
	g_map.brushes[selected_brush].set_material(&g_map.materials[0].material, 1);
	g_map.brushes[selected_brush].set_material(&g_map.materials[0].material, 2);
	g_map.brushes[selected_brush].set_material(&g_map.materials[0].material, 3);
	g_map.brushes[selected_brush].set_material(&g_map.materials[0].material, 4);
	g_map.brushes[selected_brush].set_material(&g_map.materials[0].material, 5);
	g_brush_edit_mode = BRUSH_EDIT_BRUSH;
}

static void brush_edit_mode_move_uv(Vec2 vec)
{
	if (g_brush_edit_mode == BRUSH_EDIT_PLANE)
	{
		int selected_brush = g_states[EDITOR_MODE_BRUSH].selected_object;
		int selected_plane = g_states[EDITOR_MODE_BRUSH].selected_plane;
		tek_brush_move_uv(&g_map.brushes[selected_brush], &g_map.brushes[selected_brush].planes[selected_plane],vec);
	}
}
static void brush_edit_mode_scale_uv(Vec2 vec)
{
	if (g_brush_edit_mode == BRUSH_EDIT_PLANE)
	{
		int selected_brush = g_states[EDITOR_MODE_BRUSH].selected_object;
		int selected_plane = g_states[EDITOR_MODE_BRUSH].selected_plane;
		tek_brush_scale_uv(&g_map.brushes[selected_brush],&g_map.brushes[selected_brush].planes[selected_plane],vec);
	}
}

static void brush_edit_mode_reset_uv()
{
	if (g_brush_edit_mode == BRUSH_EDIT_PLANE)
	{
		int selected_brush = g_states[EDITOR_MODE_BRUSH].selected_object;
		int selected_plane = g_states[EDITOR_MODE_BRUSH].selected_plane;
		tek_brush_reset_uv(&g_map.brushes[selected_brush],&g_map.brushes[selected_brush].planes[selected_plane]);
	}
}

static void brush_edit_mode_handle_key(Key key)
{
	if (key == KEY_LEFT)
	{
		brush_edit_mode_move(Vec3(-1,0,0));
	}
	else if (key == KEY_RIGHT)
	{
		brush_edit_mode_move(Vec3(1,0,0));
	}
	else if (key == KEY_UP)
	{
		brush_edit_mode_move(Vec3(0,0,-1));
	}
	else if (key == KEY_DOWN)
	{
		brush_edit_mode_move(Vec3(0,0,1));
	}
	else if (key == KEY_PAGE_UP)
	{
		brush_edit_mode_move(Vec3(0,1,0));
	}
	else if (key == KEY_PAGE_DOWN)
	{
		brush_edit_mode_move(Vec3(0,-1,0));
	}
	else if (key == KEY_1)
	{
		g_brush_edit_mode = BRUSH_EDIT_BRUSH;
	}
	else if (key == KEY_2)
	{
		g_brush_edit_mode = BRUSH_EDIT_PLANE;
	}
	else if (key == KEY_3)
	{
		g_brush_edit_mode = BRUSH_EDIT_LINE;
	}
	else if (key == KEY_4)
	{
		g_brush_edit_mode = BRUSH_EDIT_POINT;
	}
	else if (key == KEY_N)
	{
		brush_edit_select_next();
	}
	else if (key == KEY_P)
	{
		brush_edit_select_prev();
	}
	else if (key == KEY_B)
	{
		brush_edit_mode_add_brush();
	}
	else if (key == KEY_I)
	{
		brush_edit_mode_move_uv(vec2_create(0,0.1f));
	}
	else if (key == KEY_K)
	{
		brush_edit_mode_move_uv(vec2_create(0,-0.1f));
	}
	else if (key == KEY_J)
	{
		brush_edit_mode_move_uv(vec2_create(0.1f,0));
	}
	else if (key == KEY_L)
	{
		brush_edit_mode_move_uv(vec2_create(-0.1f,0));
	}
	else if (key == KEY_LEFT_BRACKET)
	{
		brush_edit_mode_scale_uv(vec2_create(-0.1f,0));
	}
	else if (key == KEY_RIGHT_BRACKET)
	{
		brush_edit_mode_scale_uv(vec2_create(0.1f,0));
	}
	else if (key == KEY_MINUS)
	{
		brush_edit_mode_scale_uv(vec2_create(0,-0.25f));
	}
	else if (key == KEY_EQUAL)
	{
		brush_edit_mode_scale_uv(vec2_create(0,0.25f));
	}
	else if (key == KEY_R)
	{
		brush_edit_mode_reset_uv();
	}
}

static void quad_edit_mode_move(Vec3 vec)
{
	int selected_quad = g_states[EDITOR_MODE_QUAD].selected_object;
	if (g_brush_edit_mode == BRUSH_EDIT_BRUSH)
	{
		tek_quad_move(&g_map.quads[selected_quad],vec.x, vec.y, vec.z);
	}
	else if (g_brush_edit_mode == BRUSH_EDIT_LINE)
	{
		int selected_line = g_states[EDITOR_MODE_QUAD].selected_line;
		tek_quad_move_line(&g_map.quads[selected_quad],selected_line, vec.x, vec.y, vec.z);
	}
	else if (g_brush_edit_mode == BRUSH_EDIT_POINT)
	{
		int selected_point = g_states[EDITOR_MODE_QUAD].selected_point;
		tek_quad_move_point(&g_map.quads[selected_quad],selected_point, vec.x, vec.y, vec.z);
	}
}

static void quad_edit_select_next()
{
	if (g_brush_edit_mode == BRUSH_EDIT_BRUSH)
	{
		int selected_brush = g_states[EDITOR_MODE_QUAD].selected_object;
		selected_brush++;
		if (selected_brush >= g_map.num_quads)
			selected_brush = 0;
		g_states[EDITOR_MODE_QUAD].selected_object = selected_brush;
	}
	else if (g_brush_edit_mode == BRUSH_EDIT_LINE)
	{
		int selected_line = g_states[EDITOR_MODE_QUAD].selected_line;
		selected_line++;
		if (selected_line > 3)
			selected_line = 0;
		g_states[EDITOR_MODE_QUAD].selected_line = selected_line;
	}
	else if (g_brush_edit_mode == BRUSH_EDIT_POINT)
	{
		int selected_point = g_states[EDITOR_MODE_QUAD].selected_point;
		selected_point++;
		if (selected_point > 3)
			selected_point = 0;
		g_states[EDITOR_MODE_QUAD].selected_point = selected_point;
	}
}

static void quad_edit_select_prev()
{
	if (g_brush_edit_mode == BRUSH_EDIT_BRUSH)
	{
		int selected_brush = g_states[EDITOR_MODE_QUAD].selected_object;
		selected_brush--;
		if (selected_brush < 0)
		{
			selected_brush = g_map.num_quads - 1;
		}
		g_states[EDITOR_MODE_QUAD].selected_object = selected_brush;
	}
	else if (g_brush_edit_mode == BRUSH_EDIT_LINE)
	{
		int selected_line = g_states[EDITOR_MODE_QUAD].selected_line;
		selected_line--;
		if (selected_line < 0)
			selected_line = 3;
		g_states[EDITOR_MODE_QUAD].selected_line = selected_line;
	}
	else if (g_brush_edit_mode == BRUSH_EDIT_POINT)
	{
		int selected_point = g_states[EDITOR_MODE_QUAD].selected_point;
		selected_point--;
		if (selected_point < 0)
			selected_point = 3;
		g_states[EDITOR_MODE_QUAD].selected_point = selected_point;
	}
}

static void quad_edit_mode_add_quad()
{
	TekQuad* q = tek_map_add_quad(&g_map);
	g_states[EDITOR_MODE_QUAD].selected_object = g_map.num_quads - 1;
	tek_quad_set_material(q,&g_map.materials[0].material);
	g_brush_edit_mode = BRUSH_EDIT_BRUSH;
}

static void quad_edit_mode_move_uv(Vec2 vec)
{
	if (g_brush_edit_mode == BRUSH_EDIT_BRUSH)
	{
		int selected_quad = g_states[EDITOR_MODE_BRUSH].selected_object;
		tek_quad_move_uv(&g_map.quads[selected_quad], &g_map.quads[selected_quad].plane,vec);
	}
}
static void quad_edit_mode_scale_uv(Vec2 vec)
{
	if (g_brush_edit_mode == BRUSH_EDIT_BRUSH)
	{
		int selected_quad = g_states[EDITOR_MODE_BRUSH].selected_object;
		tek_quad_scale_uv(&g_map.quads[selected_quad], &g_map.quads[selected_quad].plane,vec);
	}
}

static void quad_edit_mode_reset_uv()
{
	if (g_brush_edit_mode == BRUSH_EDIT_BRUSH)
	{
		int selected_quad = g_states[EDITOR_MODE_BRUSH].selected_object;
		tek_quad_reset_uv(&g_map.quads[selected_quad], &g_map.quads[selected_quad].plane);
	}
}

static void quad_edit_mode_handle_key(Key key)
{
	if (key == KEY_LEFT)
	{
		quad_edit_mode_move(Vec3(-1,0,0));
	}
	else if (key == KEY_RIGHT)
	{
		quad_edit_mode_move(Vec3(1,0,0));
	}
	else if (key == KEY_UP)
	{
		quad_edit_mode_move(Vec3(0,0,-1));
	}
	else if (key == KEY_DOWN)
	{
		quad_edit_mode_move(Vec3(0,0,1));
	}
	else if (key == KEY_PAGE_UP)
	{
		quad_edit_mode_move(Vec3(0,1,0));
	}
	else if (key == KEY_PAGE_DOWN)
	{
		quad_edit_mode_move(Vec3(0,-1,0));
	}
	else if (key == KEY_1)
	{
		g_brush_edit_mode = BRUSH_EDIT_BRUSH;
	}
	else if (key == KEY_2)
	{
		g_brush_edit_mode = BRUSH_EDIT_LINE;
	}
	else if (key == KEY_3)
	{
		g_brush_edit_mode = BRUSH_EDIT_POINT;
	}
	else if (key == KEY_N)
	{
		quad_edit_select_next();
	}
	else if (key == KEY_P)
	{
		quad_edit_select_prev();
	}
	else if (key == KEY_B)
	{
		quad_edit_mode_add_quad();
	}
	else if (key == KEY_I)
	{
		quad_edit_mode_move_uv(vec2_create(0,0.1f));
	}
	else if (key == KEY_K)
	{
		quad_edit_mode_move_uv(vec2_create(0,-0.1f));
	}
	else if (key == KEY_J)
	{
		quad_edit_mode_move_uv(vec2_create(0.1f,0));
	}
	else if (key == KEY_L)
	{
		quad_edit_mode_move_uv(vec2_create(-0.1f,0));
	}
	else if (key == KEY_LEFT_BRACKET)
	{
		quad_edit_mode_scale_uv(vec2_create(-0.1f,0));
	}
	else if (key == KEY_RIGHT_BRACKET)
	{
		quad_edit_mode_scale_uv(vec2_create(0.1f,0));
	}
	else if (key == KEY_MINUS)
	{
		quad_edit_mode_scale_uv(vec2_create(0,-0.1f));
	}
	else if (key == KEY_EQUAL)
	{
		quad_edit_mode_scale_uv(vec2_create(0,0.1f));
	}
	else if (key == KEY_R)
	{
		quad_edit_mode_reset_uv();
	}
}

bool tek_editor_save_map(const char* name)
{
	printf("map: %s\n",name);
	return tek_map_save(&g_map,name);
}

bool tek_editor_load_map(const char* name)
{
	return tek_map_load(&g_map,name);
}