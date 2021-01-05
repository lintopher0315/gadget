#include "FileTree.h"

FileTree::FileTree(wxWindow *parent) : wxTreeCtrl(parent, wxID_ANY, wxDefaultPosition, wxSize(wxGetDisplaySize().GetWidth()/4, wxGetDisplaySize().GetHeight()/2), wxTR_DEFAULT_STYLE, wxDefaultValidator, wxTreeCtrlNameStr) {
    wxFont *font = new wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString);
	SetFont(*font);

	cwd = getFrame()->cwd;
	root = AddRoot(cwd);
	loadTree(cwd, root);

	Bind(wxEVT_TREE_ITEM_ACTIVATED, &FileTree::onActivate, this);
}

Frame *FileTree::getFrame() const {
	return (Frame *)GetParent();
}

void FileTree::onActivate(wxTreeEvent& event) {
	std::string relPath = getRelPathFromItem(event.GetItem());
	std::string absPath = cwd + relPath;
	if (std::filesystem::is_regular_file(absPath)) {
		Window *w = getFrame()->window;
		w->panel->AddPage(new Editor(w->panel), relPath, true);
		w->getCurrentEditor()->relPath = relPath;
		w->getCurrentEditor()->LoadFile(absPath);
	}
}

void FileTree::loadTree(const std::string& cwd, wxTreeItemId parent) {
	for (const auto & file : std::filesystem::directory_iterator(cwd)) {
		wxTreeItemId item = AppendItem(parent, file.path().filename().string());
		if (std::filesystem::is_directory(file.path())) {
			loadTree(file.path().string(), item);	
		}
	}
}

// every time a file is added or deleted, reload tree
void FileTree::reloadTree() {
	DeleteChildren(root);
	loadTree(cwd, root);
}

std::string FileTree::getRelPathFromItem(const wxTreeItemId& item) const {
	std::string absPath;
	wxTreeItemId id = item;
	while (id != root) {
		absPath.insert(0, GetItemText(id).ToStdString() + "/");
		id = GetItemParent(id);
	}
	if (!absPath.empty()) {
		absPath.pop_back();
	}
	return absPath;
}
