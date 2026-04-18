#pragma once

#include <windows.h>

#include <string>

void MoveMouse(int x, int y);

void SimulateMouseClick(int x, int y);

void SendChatMessage(const std::string& message);

void UseInteractionKey();
