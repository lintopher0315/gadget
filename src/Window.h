#pragma once

#include <wx/wx.h>

#include <wx/wx.h>

#include "Editor.h"
#include "StatusBar.h"

class Window : public wxWindow {
    public:
        Window(wxWindow *parent);
        Editor *editor;
        StatusBar *statusBar;
};
