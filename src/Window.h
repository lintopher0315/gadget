#pragma once

#include <wx/wx.h>
#include <filesystem>
#include <fstream>

class Frame;
class Window;

#include "Frame.h"
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

        Frame *getFrame(void);
        Editor *getCurrentEditor(void);
        void executeNormal(int cmdInd);
        void executeCommand(int cmdInd);
        bool isValidPath(std::string relPath);

        Panel *panel;
        StatusBar *statusBar;
        CommandBar *commandBar;
        wxBoxSizer *sizer;
        Command *command;

        unsigned short mode;
        unsigned short currEditor;

        std::string cwd;
};
