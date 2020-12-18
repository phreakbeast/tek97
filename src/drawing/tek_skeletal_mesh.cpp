#include "tek_skeletal_mesh.hpp"

#include "../platform/tek_platform.hpp"
#include <stdio.h>

void tek_skeletal_mesh_destroy(TekSkeletalMesh *mesh)
{
	tek_free(mesh->vertices);
	tek_free(mesh->indices);
}

bool tek_skeletal_mesh_load(TekSkeletalMesh *mesh, const char *filename)
{
	FILE *fp = fopen(filename, "rb");
	if (!fp)
	{
		printf("Error: cannot open file %s\n", filename);
		return false;
	}

	u32 vertex_ctr = 0;
	u32 index_ctr = 0;

	char line[1024];
	while (fgets(line, 1024, fp))
	{
		if (line[0] == 'h' && line[1] == 'd')
		{
			u32 num_vertices, num_indices;

			int result = sscanf(line, "hd %u %u", &num_vertices, &num_indices);
			if (result < 2)
			{
				printf("Warning: cannot process line: %s File: %s\n", line, filename);
				fclose(fp);
				return false;
			}

			mesh->num_vertices = num_vertices;
			mesh->num_indices = num_indices;

			mesh->indices = (u32*)tek_malloc(sizeof(u32) * num_indices);
			mesh->vertices = (TekSkeletalVertexData*)tek_malloc(sizeof(TekSkeletalVertexData) * num_vertices);
		}
		else if (line[0] == 'v' && line[1] == 'x')
		{
			float x, y, z;
			float u, v;
			float nx, ny, nz;
			float r, g, b, a;
			int b0, b1, b2, b3;
			float w0, w1, w2, w3;

			int result = sscanf(line, "vx %f %f %f %f %f %f %f %f %f %f %f %f %d %d %d %d %f %f %f %f", &x, &y, &z,
			                    &u, &v, &nx, &ny, &nz,
			                    &r, &g, &b, &a, &b0, &b1, &b2, &b3, &w0, &w1, &w2, &w3);
			if (result < 20)
			{
				printf("Warning: cannot process line: %s File: %s\n", line, filename);
				fclose(fp);
				return false;
			}
			TekSkeletalVertexData vd;
			vd.position = vec3_create(x, y, z);
			vd.uv = vec2_create(u, v);
			vd.normal = vec3_create(nx, ny, nz);
			vd.color = vec4_create(r, g, b, a);
			vd.bones = vec4_create(b0, b1, b2, b3);
			vd.weights = vec4_create(w0, w1, w2, w3);

			mesh->vertices[vertex_ctr++] = vd;
		}
		else if (line[0] == 'i' && line[1] == 'd')
		{
			u32 idx;
			int result = sscanf(line, "id %u", &idx);
			if (result < 1)
			{
				printf("Warning: cannot process line: %s File: %s\n", line, filename);
				fclose(fp);
				return false;
			}

			mesh->indices[index_ctr++] = idx;
		}
	}

	fclose(fp);

	int size = mesh->num_vertices * sizeof(TekSkeletalVertexData);

	GLCall(glGenVertexArrays(1, &mesh->vao));
	GLCall(glBindVertexArray(mesh->vao));

	GLCall(glGenBuffers(1, &mesh->vbo));
	GLCall(glGenBuffers(1, &mesh->ibo));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, mesh->vertices, GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glEnableVertexAttribArray(3));
	GLCall(glEnableVertexAttribArray(4));
	GLCall(glEnableVertexAttribArray(5));

	int stride = sizeof(TekSkeletalVertexData);

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride,
	                             (const GLvoid *) (offsetof(TekSkeletalVertexData, position))));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride,
	                             (const GLvoid *) (offsetof(TekSkeletalVertexData, uv))));
	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,
	                             (const GLvoid *) (offsetof(TekSkeletalVertexData, normal))));
	GLCall(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride,
	                             (const GLvoid *) (offsetof(TekSkeletalVertexData, color))));
	GLCall(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, stride,
	                             (const GLvoid *) (offsetof(TekSkeletalVertexData, bones))));
	GLCall(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride,
	                             (const GLvoid *) (offsetof(TekSkeletalVertexData, weights))));


	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->num_indices * sizeof(u32), mesh->indices, GL_STATIC_DRAW));

	GLCall(glDisableVertexAttribArray(0));
	GLCall(glDisableVertexAttribArray(1));
	GLCall(glDisableVertexAttribArray(2));
	GLCall(glDisableVertexAttribArray(3));
	GLCall(glDisableVertexAttribArray(4));
	GLCall(glDisableVertexAttribArray(5));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));

	return true;
}
