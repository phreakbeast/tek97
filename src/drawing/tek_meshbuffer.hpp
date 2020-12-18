#ifndef _TEK_MESH_BUFFER_HPP
#define _TEK_MESH_BUFFER_HPP

#include "tek_mesh.hpp"
#include "tek_billboard.hpp"
#include "tek_material.hpp"
#include "tek_camera.hpp"
#include "tek_light.hpp"
#include "../math/tek_transform.hpp"
#include <vector>

struct TekMeshbuffer
{
	struct VertexData
	{
		Vec3 position;
		Vec2 uv;
		Vec3 normal;
		Vec4 model0;
		Vec4 model1;
		Vec4 model2;
		Vec4 model3;
		Vec4 flags;
		

		VertexData() = default;
		VertexData(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		{
			position = vec3_create(x, y, z);
			uv = vec2_create(u,v);
			normal = vec3_create(nx, ny, nz);
		}
	};

	enum class OrderType
	{
		FrontToBack,
		BackToFront
	};

	struct DrawCommand
	{
		VertexData vertices[3];
		u32 mat_id;
		u32 tex_id;
		Mat4 transform;
		float distance;
	};

	struct MaterialCommand
	{
		TekMaterial* material;
		u32 tex_id;
	};

	TekMeshbuffer(TekCamera* cam, TekLightManager* lights, OrderType order = OrderType::FrontToBack);
	~TekMeshbuffer();
	void draw_mesh(TekMesh* mesh, TekMaterial* material,TekTransform* transform);
	void draw_billboard(TekBillboard* bb, TekMaterial* material,TekTransform* transform, bool spherical = false);
	void render();
	void reset();
private:
	Vec2 add_material(TekMaterial* material);
	TekCamera* cam;
	TekLightManager* lights;
	std::vector<DrawCommand> commands;
	OrderType sort_order;
	std::vector<MaterialCommand> material_slots;
	std::vector<TekTexture*> texture_slots;
};

#endif
