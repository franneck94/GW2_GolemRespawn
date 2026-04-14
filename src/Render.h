#pragma once

#include <d3d11.h>

#include <filesystem>
#include <string>
#include <vector>

#include "imgui.h"

#include "Types.h"

class RenderType
{
public:
    RenderType() {};
    RenderType(bool &show_window) : show_window(show_window) {};
    ~RenderType();

    void render(ID3D11Device *pd3dDevice);

public:
    bool show_window;
};
