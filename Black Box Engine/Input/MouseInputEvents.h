#pragma once

enum class MouseEvent
{
    kMouseDown,
    kMouseUp,
    kMouseWheel,
    kMouseMotion,
    kCount,
};

struct MouseState
{
    float xScreenPos;
    float yScreenPos;
    bool mouseDown;
};