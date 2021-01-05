#pragma once

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <filesystem>

class FileTree;

#include "Frame.h"

class FileTree : public wxTreeCtrl {
	public:
		FileTree(wxWindow *parent);

		Frame *getFrame(void) const;
		void loadTree(const std::string& cwd, wxTreeItemId parent);
		void reloadTree(void);

		std::string cwd;
		wxTreeItemId root;
};
