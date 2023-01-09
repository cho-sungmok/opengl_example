#pragma once

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "texture.h"

CLASS_PTR(Context)
class Context {
public:
    static ContextUPtr Create();
    void Render();
private:
    Context();
    bool init();
    ProgramUPtr m_program;

	VertexLayoutUPtr m_vertexLayout;
	BufferUPtr m_vertexBuffer;
	BufferUPtr m_indexBuffer;
    TextureUPtr m_texture;
    TextureUPtr m_texture2;
};
