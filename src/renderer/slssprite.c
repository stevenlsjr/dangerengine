
/**
 * @file
 * @brief
 *
 **/

#include "slssprite.h"

slsSprite *sls_sprite_init(slsSprite *self, slsTransform2D transform)
{
  self->transform = transform;
  GLuint buffers[2];
  glGenBuffers(2, buffers);
  self->vbo = buffers[0];
  self->ibo = buffers[1];

  glGenVertexArrays(1, &self->vao);

  slsVertex2D verts[] = {
      {.position={-0.5f, -0.5f, 0.0f}, .uv={0.f, 0.f}},
      {.position={0.5f, -0.5f, 0.0f}, .uv={1.f, 0.f}},
      {.position={0.5f, 0.5f, 0.0f}, .uv={1.f, 1.f}},
      {.position={-0.5f, 0.5f, 0.0f}, .uv={0.f, 1.f}}
  };

  uint32_t idx[] = {0, 1, 2, 3, 2, 0};

  glBindVertexArray(self->vao);
  glBindBuffer(GL_ARRAY_BUFFER, self->vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ibo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);


  glVertexAttribPointer(SLS_ATTRIB_POSITION,
                        3, GL_FLOAT, false, sizeof(slsVertex2D),
                        (void *) offsetof(slsVertex2D, position));
  glVertexAttribPointer(SLS_ATTRIB_COLOR,
                        4, GL_FLOAT, false, sizeof(slsVertex2D),
                        (void *) offsetof(slsVertex2D, color));

  glVertexAttribPointer(SLS_ATTRIB_UV,
                        2, GL_FLOAT, false, sizeof(slsVertex2D),
                        (void *) offsetof(slsVertex2D, uv));
  glEnableVertexAttribArray(SLS_ATTRIB_POSITION);
  glEnableVertexAttribArray(SLS_ATTRIB_COLOR);
  glEnableVertexAttribArray(SLS_ATTRIB_UV);


  glBindVertexArray(0);

  return self;
}

slsSprite *sls_sprite_dtor(slsSprite *self)
{
  glDeleteBuffers(2, (GLuint[]) {self->vbo, self->ibo});
  glDeleteVertexArrays(1, &self->vao);
  return self;
}


void sls_sprite_draw(slsSprite *self, slsRendererGL *renderer)
{
  glBindVertexArray(self->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->vbo);
  glDrawElements(GL_POINTS, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
