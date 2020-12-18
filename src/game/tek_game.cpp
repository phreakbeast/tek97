#include "tek_game.hpp"

#include "../drawing/tek_drawing.hpp"
#include "../drawing/tek_meshbuffer.hpp"
#include "../drawing/tek_shapebuffer.hpp"
#include "../core/tek_input.hpp"
#include "../math/tek_transform.hpp"
#include "tek_map.hpp"

static TekFont g_font;
static TekCamera g_camera;
static TekMesh g_plane;
static TekMesh g_mesh;
static TekBillboard g_bb;
static TekMaterial g_material;
static TekTransform g_transform;
static TekTransform g_transform2;
static TekTransform g_transform3;

static TekLightManager g_lights;

static float g_rotate = 0.0f;

static TekMeshbuffer* g_buffer;
static TekShapebuffer* g_shapebuffer;

static TekMap g_map;

bool tek_game_init(u32 width, u32 height)
{
	if (!tek_font_load(&g_font, "data/fonts/test.font"))
		return false;

	Mat4 projection = mat4_perspective(45.0f, (float)width / height, 0.1f, 100.0f);
	g_camera = tek_cam_create(TEK_CAM_THIRD, projection);

	tek_mesh_create_box(&g_mesh, 1, 1, 1);
	tek_mesh_create_plane(&g_plane, 10, 10, 5, 5);

	tek_bb_init(&g_bb, 3, 2, tek_rect_create(0, 0, 1, 1));

	tek_material_init(&g_material);
	tek_material_add_diffuse_map(&g_material, "data/textures/grid.tga");

	g_transform = tek_transform_create(vec3_create(0, 0, 0), vec3_create(0, 0, 0), vec3_create(1, 1, 1));
	g_transform2 = tek_transform_create(vec3_create(-5, 0, -5), vec3_create(0, 0, 0), vec3_create(1, 1, 1));
	g_transform3 = tek_transform_create(vec3_create(2, 0, 2), vec3_create(0, 0, 0), vec3_create(1, 1, 1));


	g_lights.use_dlight = true;
	g_lights.dlight.color = tek_color_to_vec3(tek_color_create(10, 10, 10));
	g_lights.dlight.direction = vec3_create(0, -0.75f, -0.25f);
	g_lights.dlight.intensity = 0.1f;

	g_lights.num_plights = 2;
	g_lights.plights[0].color = tek_color_to_vec3(tek_color_white());
	g_lights.plights[0].position = vec3_create(-1, 1, 0);
	g_lights.plights[0].range = 3.0f;

	g_lights.plights[1].color = tek_color_to_vec3(tek_color_blue());
	g_lights.plights[1].position = vec3_create(1, 1, 0);
	g_lights.plights[1].range = 2.0f;

	g_buffer = new TekMeshbuffer(&g_camera,&g_lights);
	g_shapebuffer = new TekShapebuffer(&g_camera);

	tek_map_load(&g_map);

	return true;
}

void tek_game_destroy()
{
	tek_map_destroy(&g_map);
	tek_font_destroy(&g_font);
	delete g_buffer;
	delete g_shapebuffer;
}

void tek_game_render()
{
	//render 3d
	g_buffer->reset();
	g_shapebuffer->reset();
	//tek_renderer_draw_mesh(&g_plane, &g_material, &g_transform2.matrix, &g_camera, nullptr,nullptr,0);
	//tek_renderer_draw_mesh(&g_mesh, &g_material, &g_transform.matrix, &g_camera, &g_sun,g_lights,2);

	tek_map_render(&g_map, g_buffer);
	g_shapebuffer->draw_line(vec3_create(0,0,0), vec3_create(0,3,0), tek_color_green());
	//g_buffer->draw_mesh(&g_plane, &g_material, &g_transform2);
	//g_buffer->draw_mesh(&g_mesh, &g_material, &g_transform);

	g_shapebuffer->render();
	g_buffer->render();
	//tek_renderer_draw_billboard(&g_bb, &g_material, &g_transform3.matrix, &g_camera, false);

	tek_renderer_disable_depth_test();
	//render 2d
	TekSpritebatch* sb = tek_renderer_get_sb();

	//render gui
	tek_sb_begin(sb);

	//render debug text
	TekRenderStats* stats = tek_renderer_get_stats();
	char fps_str[256];
	
	sprintf(fps_str, "fps: %u ups: %u", stats->fps, stats->ups);
	
	tek_sb_render_text(sb, fps_str, &g_font, 5, 5,tek_color_white(), 0);

	tek_sb_end(sb);
	tek_sb_flush(sb);
	tek_renderer_enable_depth_test();
}

void tek_game_update(float delta)
{
	TekKeyboardState* kstate = tek_input_get_key_state();

	if (kstate->keys_down[KEY_W])
	{
		tek_cam_move(&g_camera, 0, delta * 10);
	}

	if (kstate->keys_down[KEY_S])
	{
		tek_cam_move(&g_camera, 180, delta * 10);
	}

	if (kstate->keys_down[KEY_A])
	{
		tek_cam_move(&g_camera, 90, delta * 10);
	}

	if (kstate->keys_down[KEY_D])
	{
		tek_cam_move(&g_camera, 270, delta * 10);
	}

	if (kstate->keys_down[KEY_Q])
	{
		tek_cam_rotate_y(&g_camera, delta * 10);
	}

	if (kstate->keys_down[KEY_E])
	{
		tek_cam_rotate_y(&g_camera, -delta * 10);
	}

	tek_map_update(&g_map, delta);

	g_rotate += delta * 10.0f;
	g_transform.rotation.y = g_rotate;
	tek_transform_calc(&g_transform);
	tek_cam_calc(&g_camera);
}