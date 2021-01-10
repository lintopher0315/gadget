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
		void reloadTree(void);

	private:
		void onActivate(wxTreeEvent& event);
		void onKillFocus(wxFocusEvent& event);
		void onClick(wxMouseEvent& event);

		void loadTree(const std::string& cwd, wxTreeItemId parent);
		std::string getRelPathFromItem(const wxTreeItemId& item) const;
		wxColour getColorFromExtension(const std::string& ext) const;

		std::string cwd;
		wxTreeItemId root;
};
