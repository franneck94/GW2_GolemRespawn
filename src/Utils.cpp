#include <windows.h>

#include <string>

#include "Utils.h"


void SimulateMouseClick(int x, int y)
{
    POINT originalPos;
    GetCursorPos(&originalPos);

    SetCursorPos(x, y);

    Sleep(10);

    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    Sleep(50);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

    Sleep(10);

    SetCursorPos(originalPos.x, originalPos.y);
}

void SendChatMessage(const std::string& message)
{
    keybd_event(VK_RETURN, 0, 0, 0);
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);

    Sleep(100);

    for (char c : message)
    {
        SHORT vk = VkKeyScan(c);
        BYTE virtualKey = LOBYTE(vk);
        BYTE shiftState = HIBYTE(vk);

        if (shiftState & 1) // Shift required
        {
            keybd_event(VK_SHIFT, 0, 0, 0);
        }

        keybd_event(virtualKey, 0, 0, 0);
        keybd_event(virtualKey, 0, KEYEVENTF_KEYUP, 0);

        if (shiftState & 1) // Release shift
        {
            keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
        }

        Sleep(20); // Small delay between keystrokes
    }

    Sleep(50);
    keybd_event(VK_RETURN, 0, 0, 0);
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
}
