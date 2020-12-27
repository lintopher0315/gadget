#pragma once

#include <wx/wx.h>
#include <filesystem>

#include "Panel.h"
#include "StatusBar.h"
#include "CommandBar.h"
#include "Command.h"

#define NORMAL_MODE 0
#define EDIT_MODE 1
#define GADGET_MODE 2

class Window : public wxWindow {
    public:
        Window(wxWindow *parent);

        Editor *getCurrentEditor();
        void executeCommand(int cmdInd);

        Panel *panel;
        StatusBar *statusBar;
        CommandBar *commandBar;
        wxBoxSizer *sizer;
        Command *command;

        unsigned short mode;
        unsigned short currEditor;

        std::string cwd;
};
