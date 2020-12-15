#pragma once

#include <wx/wx.h>
#include <wx/stc/stc.h>

class Window;
class Editor;

#include "Window.h"

class Editor : public wxStyledTextCtrl {
    public:
        Editor(wxWindow *parent);

        Window *getWindow();
        void onChar(wxKeyEvent& event);
};
