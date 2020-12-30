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

        int linePos(void) const;
        int lineStartPos(void) const;
        int lineEndPos(void) const;
        void caretLeft(const int& num);
        void caretRight(const int& num);
        void caretUp(const int& num);
        void caretDown(const int& num);
        void append(void);
        void insertLineBelow(const int& num);
        void insertLineAbove(const int& num);

        std::string relPath;
};
