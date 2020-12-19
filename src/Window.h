#pragma once

#include <wx/wx.h>

#include "Editor.h"
#include "StatusBar.h"
#include "Command.h"

#define NORMAL_MODE 0
#define EDIT_MODE 1
#define GADGET_MODE 2

class Window : public wxWindow {
    public:
        Window(wxWindow *parent);

        //void executeCommand(); // execute the command object we'll add later

        Editor *editor;
        StatusBar *statusBar;
        wxBoxSizer *sizer;
        Command *command;

        unsigned short mode;
};
