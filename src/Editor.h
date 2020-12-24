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
        void onKey(wxKeyEvent& event);
        void repeatAction(int num, void (*f)());

        void caretLeft(int num);
        void caretRight(int num);
        void caretUp(int num);
        void caretDown(int num);
};
