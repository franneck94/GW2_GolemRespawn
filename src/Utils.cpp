#include <windows.h>

#include <string>
#include <vector>

#include "Utils.h"


void MoveMouse(int x, int y)
{
    SetCursorPos(x, y);
}

void SimulateMouseClick(int x, int y)
{
    POINT originalPos;
    GetCursorPos(&originalPos);

    SetCursorPos(x, y);
    Sleep(100);

    INPUT inputs[2] = {};

    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dx = 0;
    inputs[0].mi.dy = 0;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    for (int i = 0; i < 20; ++i)
        SendInput(1, inputs, sizeof(INPUT));

    Sleep(100);

    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dx = 0;
    inputs[1].mi.dy = 0;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(1, &inputs[1], sizeof(INPUT));

    Sleep(100);

    SetCursorPos(originalPos.x, originalPos.y);
}

void SendChatMessage(const std::string &message)
{
    INPUT enterDown = {};
    enterDown.type = INPUT_KEYBOARD;
    enterDown.ki.wVk = 0;
    enterDown.ki.wScan = 0x1C;
    enterDown.ki.dwFlags = KEYEVENTF_SCANCODE;

    INPUT enterUp = {};
    enterUp.type = INPUT_KEYBOARD;
    enterUp.ki.wVk = 0;
    enterUp.ki.wScan = 0x1C;
    enterUp.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

    INPUT openChat[2] = { enterDown, enterUp };
    SendInput(2, openChat, sizeof(INPUT));
    Sleep(300);

    std::vector<INPUT> inputs;

    for (char c : message)
    {
        SHORT vkScan = VkKeyScanA(c);
        BYTE vk = LOBYTE(vkScan);
        BYTE shiftState = HIBYTE(vkScan);

        // Press shift if needed
        if (shiftState & 1)
        {
            INPUT shiftDown = {};
            shiftDown.type = INPUT_KEYBOARD;
            shiftDown.ki.wVk = VK_SHIFT;
            shiftDown.ki.dwFlags = 0;
            inputs.push_back(shiftDown);
        }

        // Press the character key
        INPUT keyDown = {};
        keyDown.type = INPUT_KEYBOARD;
        keyDown.ki.wVk = vk;
        keyDown.ki.dwFlags = 0;
        inputs.push_back(keyDown);

        INPUT keyUp = {};
        keyUp.type = INPUT_KEYBOARD;
        keyUp.ki.wVk = vk;
        keyUp.ki.dwFlags = KEYEVENTF_KEYUP;
        inputs.push_back(keyUp);

        // Release shift if it was pressed
        if (shiftState & 1)
        {
            INPUT shiftUp = {};
            shiftUp.type = INPUT_KEYBOARD;
            shiftUp.ki.wVk = VK_SHIFT;
            shiftUp.ki.dwFlags = KEYEVENTF_KEYUP;
            inputs.push_back(shiftUp);
        }
    }

    SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));
    Sleep(100);

    INPUT sendChat[2] = { enterDown, enterUp };
    SendInput(2, sendChat, sizeof(INPUT));
}

void UseInteractionKey()
{
    INPUT interactDown = {};
    interactDown.type = INPUT_KEYBOARD;
    interactDown.ki.wVk = 0;
    interactDown.ki.wScan = 0x21;
    interactDown.ki.dwFlags = KEYEVENTF_SCANCODE;

    INPUT interactUp = {};
    interactUp.type = INPUT_KEYBOARD;
    interactUp.ki.wVk = 0;
    interactUp.ki.wScan = 0x21;
    interactUp.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

    const int holdMs = 100;
    const int repeatIntervalMs = 16;
    SendInput(1, &interactDown, sizeof(INPUT));
    for (int elapsed = 0; elapsed < holdMs; elapsed += repeatIntervalMs)
    {
        Sleep(repeatIntervalMs);
        SendInput(1, &interactDown, sizeof(INPUT));
    }
    SendInput(1, &interactUp, sizeof(INPUT));
}
