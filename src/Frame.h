#pragma once

#include <wx/wx.h>
#include <filesystem>

#include "Window.h"

class Frame : public wxFrame {
    public:
        Frame(const wxSize& size);

        Window *window;
		FileTree *tree;

		std::string cwd;

	private:
		wxBoxSizer *sizer;
};
