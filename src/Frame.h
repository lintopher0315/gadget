#pragma once

#include <wx/wx.h>
#include <filesystem>

#include "Window.h"
#include "FileTree.h"

class Frame : public wxFrame {
    public:
        Frame(const wxSize& size);

        Window *window;
		FileTree *tree;
		wxBoxSizer *sizer;

		std::string cwd;
};
