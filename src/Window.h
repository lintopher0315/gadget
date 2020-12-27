#pragma once

#include <wx/wx.h>
#include <filesystem>

#include "Editor.h"
#include "StatusBar.h"
#include "CommandBar.h"
#include "Command.h"

#define NORMAL_MODE 0
#define EDIT_MODE 1
#define GADGET_MODE 2

class Window : public wxWindow {
    public:
        Window(wxWindow *parent);

        void executeCommand(int cmdInd);

        Editor *editor;
        StatusBar *statusBar;
        CommandBar *commandBar;
        wxBoxSizer *sizer;
        Command *command;

        unsigned short mode;

        std::string cwd;
};
