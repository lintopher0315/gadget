#pragma once

#include <wx/wx.h>

#include "Window.h"

class Frame : public wxFrame {
    public:
        Frame(const wxSize& size);
        Window *window;
};
