#include "tek_mesh.hpp"

#include "../platform/tek_platform.hpp"
#include "../drawing/tek_drawing.hpp"
   
void tek_mesh_destroy(TekMesh* mesh)
{
	
}

void tek_mesh_create(TekMesh* mesh, TekMeshVertexData* vertices, u32 num_vertices, u32* indices, u32 num_indices)
{
    mesh->num_vertices = num_vertices;
    mesh->num_indices = num_indices;

    int size = num_vertices * sizeof(TekMeshVertexData);

    GLCall(glGenVertexArrays(1, &mesh->vao));
    GLCall(glBindVertexArray(mesh->vao));

    GLCall(glGenBuffers(1, &mesh->vbo));
    GLCall(glGenBuffers(1, &mesh->ibo));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));

    Shader* shader = renderer_get_geometry_shader();
    int loc_position = shader->get_attrib_loc("a_position");
    int loc_uv = shader->get_attrib_loc("a_uv");
    int loc_normal = shader->get_attrib_loc("a_normal");
	
    GLCall(glEnableVertexAttribArray(loc_position));
    GLCall(glEnableVertexAttribArray(loc_uv));
    GLCall(glEnableVertexAttribArray(loc_normal));

    int stride = sizeof(TekMeshVertexData);

    GLCall(glVertexAttribPointer(loc_position, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(offsetof(TekMeshVertexData, position))));
    GLCall(glVertexAttribPointer(loc_uv, 2, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(offsetof(TekMeshVertexData, uv))));
    GLCall(glVertexAttribPointer(loc_normal, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(offsetof(TekMeshVertexData, normal))));


    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(u32), indices, GL_STATIC_DRAW));

    GLCall(glDisableVertexAttribArray(loc_position));
    GLCall(glDisableVertexAttribArray(loc_uv));
    GLCall(glDisableVertexAttribArray(loc_normal));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}

void tek_mesh_render(TekMesh* mesh)
{
    Shader* shader = renderer_get_geometry_shader();
    int loc_position = shader->get_attrib_loc("a_position");
    int loc_uv = shader->get_attrib_loc("a_uv");
    int loc_normal = shader->get_attrib_loc("a_normal");
	
    GLCall(glBindVertexArray(mesh->vao));

    GLCall(glEnableVertexAttribArray(loc_position));
    GLCall(glEnableVertexAttribArray(loc_uv));
    GLCall(glEnableVertexAttribArray(loc_normal));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo));
    GLCall(glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, 0));

    GLCall(glDisableVertexAttribArray(loc_position));
    GLCall(glDisableVertexAttribArray(loc_uv));
    GLCall(glDisableVertexAttribArray(loc_normal));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}
