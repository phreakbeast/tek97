#include "tek_geometry.hpp"

#include <stdio.h>

void tek_geometry_mesh_plane(TekMesh* mesh, float width, float height)
{
    TekMeshVertexData vertices[4];
    u32 indices[6];

    float w = width / 2.0f;
    float d = height / 2.0f;

    vertices[0] = tek_mesh_vertex_create(vec3_create(-w, 0,-d), vec2_create(0, 1), vec3_create(0, 1, 0));
    vertices[1] = tek_mesh_vertex_create(vec3_create(-w, 0, d), vec2_create(0, 0), vec3_create(0, 1, 0));
    vertices[2] = tek_mesh_vertex_create(vec3_create( w, 0, d), vec2_create(1, 0), vec3_create(0, 1, 0));
    vertices[3] = tek_mesh_vertex_create(vec3_create( w, 0,-d), vec2_create(1, 1), vec3_create(0, 1, 0));

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 3;
	
    tek_mesh_create(mesh, vertices, 4, indices, 6);
}


void tek_geometry_mesh_create_box(TekMesh* mesh, float width, float height, float depth)
{
    TekMeshVertexData vertices[24];
    u32 indices[36];

    float w = width / 2.0f;
    float d = depth / 2.0f;
    float h = height;

    //front
    vertices[0] = tek_mesh_vertex_create( vec3_create(-w, h, d), vec2_create(0, 1), vec3_create(0, 0, 1) );
    vertices[1] = tek_mesh_vertex_create( vec3_create(-w, 0, d), vec2_create(0, 0), vec3_create(0, 0, 1) );
    vertices[2] = tek_mesh_vertex_create( vec3_create( w, 0, d), vec2_create(1, 0), vec3_create(0, 0, 1) );
    vertices[3] = tek_mesh_vertex_create( vec3_create( w, h, d), vec2_create(1, 1), vec3_create(0, 0, 1) );

    //left
    vertices[4] = tek_mesh_vertex_create( vec3_create(-w, h,-d), vec2_create(0, 1), vec3_create(-1, 0, 0) );
    vertices[5] = tek_mesh_vertex_create( vec3_create(-w, 0,-d), vec2_create(0, 0), vec3_create(-1, 0, 0) );
    vertices[6] = tek_mesh_vertex_create( vec3_create(-w, 0, d), vec2_create(1, 0), vec3_create(-1, 0, 0) );
    vertices[7] = tek_mesh_vertex_create( vec3_create(-w, h, d), vec2_create(1, 1), vec3_create(-1, 0, 0) );

    //right
    vertices[8]  = tek_mesh_vertex_create( vec3_create(w, h, d), vec2_create(0, 1), vec3_create(1, 0, 0) );
    vertices[9]  = tek_mesh_vertex_create( vec3_create(w, 0, d), vec2_create(0, 0), vec3_create(1, 0, 0) );
    vertices[10] = tek_mesh_vertex_create( vec3_create(w, 0,-d), vec2_create(1, 0), vec3_create(1, 0, 0) );
    vertices[11] = tek_mesh_vertex_create( vec3_create(w, h,-d), vec2_create(1, 1), vec3_create(1, 0, 0) );

    //back
    vertices[12] = tek_mesh_vertex_create( vec3_create( w, h,-d), vec2_create(0, 1), vec3_create(0, 0, -1) );
    vertices[13] = tek_mesh_vertex_create( vec3_create( w, 0,-d), vec2_create(0, 0), vec3_create(0, 0, -1) );
    vertices[14] = tek_mesh_vertex_create( vec3_create(-w, 0,-d), vec2_create(1, 0), vec3_create(0, 0, -1) );
    vertices[15] = tek_mesh_vertex_create( vec3_create(-w, h,-d), vec2_create(1, 1), vec3_create(0, 0, -1) );

    //top
    vertices[16] = tek_mesh_vertex_create( vec3_create(-w, h,-d), vec2_create(0, 1), vec3_create(0, 1, 0) );
    vertices[17] = tek_mesh_vertex_create( vec3_create(-w, h, d), vec2_create(0, 0), vec3_create(0, 1, 0) );
    vertices[18] = tek_mesh_vertex_create( vec3_create( w, h, d), vec2_create(1, 0), vec3_create(0, 1, 0) );
    vertices[19] = tek_mesh_vertex_create( vec3_create( w, h,-d), vec2_create(1, 1), vec3_create(0, 1, 0) );

    //bottom
    vertices[20] = tek_mesh_vertex_create(vec3_create(-w, 0, d), vec2_create(0, 1), vec3_create(0, -1, 0));
    vertices[21] = tek_mesh_vertex_create(vec3_create(-w, 0,-d), vec2_create(0, 0), vec3_create(0, -1, 0));
    vertices[22] = tek_mesh_vertex_create(vec3_create( w, 0,-d), vec2_create(1, 0), vec3_create(0, -1, 0));
    vertices[23] = tek_mesh_vertex_create(vec3_create( w, 0, d), vec2_create(1, 1), vec3_create(0, -1, 0));

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 3;

    indices[6] = 4;
    indices[7] = 5;
    indices[8] = 6;
    indices[9] = 4;
    indices[10] = 6;
    indices[11] = 7;

    indices[12] = 8;
    indices[13] = 9;
    indices[14] = 10;
    indices[15] = 8;
    indices[16] = 10;
    indices[17] = 11;

    indices[18] = 12;
    indices[19] = 13;
    indices[20] = 14;
    indices[21] = 12;
    indices[22] = 14;
    indices[23] = 15;

    indices[24] = 16;
    indices[25] = 17;
    indices[26] = 18;
    indices[27] = 16;
    indices[28] = 18;
    indices[29] = 19;

    indices[30] = 20;
    indices[31] = 21;
    indices[32] = 22;
    indices[33] = 20;
    indices[34] = 22;
    indices[35] = 23;
	
    tek_mesh_create(mesh, vertices, 24, indices, 36);
}


void tek_mesh_create_cylinder(TekMesh* mesh, float rad, int segment, float h, float uv_x, float uv_y)
{
    TekMeshVertexData* vertices = (TekMeshVertexData*)tek_malloc(sizeof(TekMeshVertexData) * segment * 6);
    u32* indices = (u32*)tek_malloc(sizeof(u32) * segment * 12);

    Vec2* points = (Vec2*)tek_malloc(sizeof(Vec2) * segment);

    u32 point_ctr = 0;
    u32 vertex_ctr = 0;
    u32 index_ctr = 0;

    float seg = 360.0f / segment;
    for (int i = 0; i < segment; ++i)
    {
	float x, y;
	float angle = seg * i;

	math_get_circle_coord(rad, 0, 0, angle, &x, &y);
	Vec2 v = { x,y };
	points[point_ctr++] = v;
    }

    for (int i = 0; i < segment; ++i)
    {
	float x1 = points[i].x;
	float y1 = points[i].y;

	float x2, y2;
	if (i == segment - 1)
	{
	    x2 = points[0].x;
	    y2 = points[0].y;
	}
	else
	{
	    x2 = points[i + 1].x;
	    y2 = points[i + 1].y;
	}

	float fixed_uv_x1 = ((float)i / segment) * uv_x;
	float fixed_uv_x2;

	if (i == segment - 1)
	{
	    fixed_uv_x2 = 1 * uv_x;
	}
	else
	{
	    fixed_uv_x2 = ((float)(i + 1) / segment) * uv_x;
	}

	u32 vert_c = vertex_ctr;

	Vec3 n1 = { x1, 0, y1 };
	Vec3 n2 = { x2, 0, y2 };
	n1 = vec3_normalized(n1);
	n2 = vec3_normalized(n2);

	vertices[vertex_ctr++] = tek_mesh_vertex_create(vec3_create(x2, h, y2), vec2_create(fixed_uv_x2, 0),vec3_create(n2.x, n2.y, n2.z)); //0
	vertices[vertex_ctr++] = tek_mesh_vertex_create(vec3_create(x2, 0, y2), vec2_create(fixed_uv_x2, uv_y), vec3_create(n2.x, n2.y, n2.z)); //1

	vertices[vertex_ctr++] = tek_mesh_vertex_create(vec3_create(x1, 0, y1), vec2_create(fixed_uv_x1, uv_y), vec3_create(n1.x, n1.y, n1.z)); //2
	vertices[vertex_ctr++] = tek_mesh_vertex_create(vec3_create(x1, h, y1), vec2_create(fixed_uv_x1,    0), vec3_create(n1.x, n1.y, n1.z)); //3

	vertices[vertex_ctr++] = tek_mesh_vertex_create(vec3_create(0, h, 0), vec2_create(0, 0), vec3_create(0, 1, 0)); //4
	vertices[vertex_ctr++] = tek_mesh_vertex_create(vec3_create(0, 0, 0), vec2_create(0, 0), vec3_create(0, -1, 0)); //5

	indices[index_ctr++] = (vert_c + 0);
	indices[index_ctr++] = (vert_c + 1);
	indices[index_ctr++] = (vert_c + 2);

	indices[index_ctr++] = (vert_c + 0);
	indices[index_ctr++] = (vert_c + 2);
	indices[index_ctr++] = (vert_c + 3);

	indices[index_ctr++] = (vert_c + 4);
	indices[index_ctr++] = (vert_c + 0);
	indices[index_ctr++] = (vert_c + 3);

	indices[index_ctr++] = (vert_c + 1);
	indices[index_ctr++] = (vert_c + 5);
	indices[index_ctr++] = (vert_c + 2);
    }
	
    tek_mesh_create(mesh, vertices, vertex_ctr, indices, index_ctr);

    tek_free(points);
    tek_free(vertices);
    tek_free(indices);
}


bool tek_geometry_mesh_load(TekMesh* mesh, const char* filename)
{
    FILE* fp = fopen(filename, "rb");
    if (!fp)
    {
	printf("Error: cannot open file %s\n", filename);
	return false;
    }

    u32 num_position = 0;
    u32 num_normals = 0;
    u32 num_uvs = 0;
    u32 num_faces = 0;

    char line[1024];
    while (fgets(line, 1024, fp))
    {
	if (line[0] == 'v' && line[1] == ' ')
	{
	    num_position++;
	}
	else if (line[0] == 'v' && line[1] == 'n')
	{
	    num_normals++;
	}
	else if (line[0] == 'v' && line[1] == 't')
	{
	    num_uvs++;
	}
	else if (line[0] == 'f' && line[1] == ' ')
	{
	    num_faces++;
	}
    }

    Vec3* temp_positions = (Vec3*)tek_malloc(sizeof(Vec3) * num_position);
    Vec3* temp_normals   = (Vec3*)tek_malloc(sizeof(Vec3) * num_normals);
    Vec3* temp_uvs       = (Vec3*)tek_malloc(sizeof(Vec3) * num_uvs);

    TekMeshVertexData* vertices = (TekMeshVertexData*)tek_malloc(sizeof(TekMeshVertexData) * num_faces * 3);
    u32* indices = (u32*)tek_malloc(sizeof(u32) * num_faces * 3);

    u32 position_ctr = 0;
    u32 normal_ctr = 0;
    u32 uv_ctr = 0;
    u32 vertex_ctr = 0;
    u32 index_ctr = 0;

    fseek(fp, 0, SEEK_SET);
    while (fgets(line, 1024, fp))
    {
	//std::cout << line << std::endl;
	if (line[0] == '\n')
	    continue;

	if (line[0] == 'v' && line[1] == ' ')
	{

	    float x, y, z;

	    int res = sscanf(line, "v %f %f %f", &x, &y, &z);
	    if (res < 3)
	    {
		printf("Warning: cannot process line: %s File: %s\n", line, filename);
		fclose(fp);
		return 0;
	    }

	    Vec3 v = { x,y,z };
	    temp_positions[position_ctr++] = v;
	}
	else if (line[0] == 'v' && line[1] == 'n')
	{
	    float x, y, z;

	    int res = sscanf(line, "vn %f %f %f", &x, &y, &z);
	    if (res < 3)
	    {
		printf("Warning: cannot process line: %s File: %s\n", line, filename);
		fclose(fp);
		return 0;
	    }

	    Vec3 v = { x,y,z };
	    temp_normals[normal_ctr++] = v;
	}
	else if (line[0] == 'v' && line[1] == 't')
	{
	    float x, y;

	    int res = sscanf(line, "vt %f %f", &x, &y);
	    if (res < 2)
	    {
		printf("Warning: cannot process line: %s File: %s\n", line, filename);
		fclose(fp);
		return 0;
	    }

	    Vec3 v = { x,y,0 };
	    temp_uvs[uv_ctr++] = v;
	}
	else if (line[0] == 'f' && line[1] == ' ')
	{
	    int v0, t0, n0;
	    int v1, t1, n1;
	    int v2, t2, n2;

	    int res = sscanf(&line[0], "f %d/%d/%d %d/%d/%d %d/%d/%d",
			     &v0, &t0, &n0, &v1, &t1, &n1, &v2, &t2, &n2);
	    if (res < 9)
	    {
		printf("Warning: cannot process line: %s File: %s\n", line, filename);
		fclose(fp);
		return 0;
	    }

	    TekMeshVertexData vert0;
	    vert0.position = temp_positions[v0 - 1];
	    vert0.normal = temp_normals[n0 - 1];
	    vert0.uv.x = temp_uvs[t0 - 1].x;
	    vert0.uv.y = temp_uvs[t0 - 1].y;

	    vertices[vertex_ctr] = vert0;
	    indices[index_ctr++] = vertex_ctr++;

	    TekMeshVertexData vert1;
	    vert1.position = temp_positions[v1 - 1];
	    vert1.normal = temp_normals[n1 - 1];
	    vert1.uv.x = temp_uvs[t1 - 1].x;
	    vert1.uv.y = temp_uvs[t1 - 1].y;

	    vertices[vertex_ctr] = vert1;
	    indices[index_ctr++] = vertex_ctr++;

	    TekMeshVertexData vert2;
	    vert2.position = temp_positions[v2 - 1];
	    vert2.normal = temp_normals[n2 - 1];
	    vert2.uv.x = temp_uvs[t2 - 1].x;
	    vert2.uv.y = temp_uvs[t2 - 1].y;

	    vertices[vertex_ctr] = vert2;
	    indices[index_ctr++] = vertex_ctr++;
	}
    }

    fclose(fp);

    printf("Mesh file proccessed. Creating geometry ...\n");

    tek_mesh_create(mesh, vertices, vertex_ctr, indices, index_ctr);
	
    tek_free(temp_positions);
    tek_free(temp_normals);
    tek_free(temp_uvs);
    tek_free(vertices);
    tek_free(indices);

    return true;
}
