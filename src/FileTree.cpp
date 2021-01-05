#include "FileTree.h"

FileTree::FileTree(wxWindow *parent) : wxTreeCtrl(parent, wxID_ANY, wxDefaultPosition, wxSize(wxGetDisplaySize().GetWidth()/4, wxGetDisplaySize().GetHeight()/2), wxTR_DEFAULT_STYLE, wxDefaultValidator, wxTreeCtrlNameStr) {
    wxFont *font = new wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString);
	SetFont(*font);

	cwd = getFrame()->cwd;
	root = AddRoot(cwd);
	loadTree(cwd, root);
}

Frame *FileTree::getFrame() const {
	return (Frame *)GetParent();
}

void FileTree::loadTree(const std::string& cwd, wxTreeItemId parent) {
	for (const auto & file : std::filesystem::directory_iterator(cwd)) {
		wxTreeItemId item = AppendItem(parent, file.path().filename().string());
		if (is_directory(file.path())) {
			loadTree(file.path().string(), item);	
		}
	}
}

// every time a file is added or deleted, reload tree
void FileTree::reloadTree() {
	DeleteChildren(root);
	loadTree(cwd, root);
}
