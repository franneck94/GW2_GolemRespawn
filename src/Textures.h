#pragma once

#include <d3d11.h>

#include <filesystem>
#include <string>
#include <unordered_map>

#include "nexus/Nexus.h"

#include "Types.h"

using TextureMapType = std::unordered_map<int, ID3D11ShaderResourceView *>;

ID3D11ShaderResourceView *LoadTextureFromPNG_WIC(ID3D11Device *device, const std::wstring &filename);

void ReleaseTextureMap(TextureMapType &texture_map);
