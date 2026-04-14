#include <windows.h>

#include <commdlg.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "ole32.lib")
#include <d3d11.h>

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <map>
#include <numbers>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <utility>

#include "imgui.h"

#include "mumble/Mumble.h"

#include "Defines.h"
#include "Render.h"
#include "Settings.h"
#include "Shared.h"
#include "Textures.h"
#include "Types.h"
#include "Version.h"

RenderType::~RenderType()
{
}

void RenderType::render(ID3D11Device *pd3dDevice)
{
    Globals::RenderData.pd3dDevice = pd3dDevice;

    if (!Settings::ShowWindow)
        return;
}
