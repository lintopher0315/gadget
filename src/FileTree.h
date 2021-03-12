#pragma once

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <filesystem>
#include <functional>

class Frame;
class Window;

class FileTree : public wxTreeCtrl {
	public:
		FileTree(wxWindow *parent);

		Frame *getFrame(void) const;
		void reloadTree(void);

	private:
		void onActivate(wxTreeEvent& event);
		void onKillFocus(wxFocusEvent& event);
		void onClick(wxMouseEvent& event);

		int loadTree(const std::string& cwd, wxTreeItemId parent);
		std::string getRelPathFromItem(const wxTreeItemId& item) const;
		wxColour getColorFromExtension(const std::string& ext) const;
		wxColour getDirectoryColor(const int& num) const;

		int OnCompareItems(const wxTreeItemId& item1, const wxTreeItemId& item2);

		std::string cwd;
		wxTreeItemId root;
};
