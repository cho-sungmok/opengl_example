#pragma once

#include "common.h"
#include "shader.h"
#include "program.h"

CLASS_PTR(Context)
class Context {
public:
    static ContextUPtr Create();
    void Render();
private:
    Context();
    bool init();
    ProgramUPtr m_program;
};
