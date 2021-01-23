#pragma once

#include <wx/wx.h>
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

        Frame *getFrame(void) const;
        Editor *getCurrentEditor(void) const;
        void executeNormal(const int& cmdInd);
        void executeCommand(const int& cmdInd);
		void updateStatus(void);

        Panel *panel;
        StatusBar *statusBar;
        CommandBar *commandBar;
        Command *command;

        unsigned short mode;
        unsigned short currEditor;

	private:
		void doInsertion(void);
		void doBasicMovement(void);
		void doNewLine(void);
		void doIntraLineJump(void);
		void doInterLineJump(void);
		void doTabChange(void);
		void doWordJump(void);

		void doQuitFile(void);
		void doSaveFile(void);
		void doOpenFile(void);
		void doNewTab(void);
		void doSplitTab(void);
        bool isExistingPath(const std::string& relPath) const;
        bool isValidPath(const std::string& relPath) const;

        wxBoxSizer *sizer;
};
