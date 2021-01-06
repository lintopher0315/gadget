#pragma once

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <filesystem>
#include <functional>

class FileTree;

#include "Frame.h"

class FileTree : public wxTreeCtrl {
	public:
		FileTree(wxWindow *parent);

		Frame *getFrame(void) const;
		void onActivate(wxTreeEvent& event);
		void loadTree(const std::string& cwd, wxTreeItemId parent);
		void reloadTree(void);
		std::string getRelPathFromItem(const wxTreeItemId& item) const;
		wxColour getColorFromExtension(const std::string& ext) const;

		std::string cwd;
		wxTreeItemId root;
};
