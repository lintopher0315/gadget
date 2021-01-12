#include "FileTree.h"

FileTree::FileTree(wxWindow *parent) : wxTreeCtrl(parent, wxID_ANY, wxDefaultPosition, wxSize(wxGetDisplaySize().GetWidth()/4, wxGetDisplaySize().GetHeight()/2), wxTR_DEFAULT_STYLE | wxTR_FULL_ROW_HIGHLIGHT | wxTR_NO_LINES, wxDefaultValidator, wxTreeCtrlNameStr) {
    wxFont *font = new wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString);
	SetFont(*font);
	SetMinClientSize(wxSize(100, 100));
	SetBackgroundColour(wxColour(37, 37, 38));
	SetIndent(20);

	cwd = getFrame()->cwd;
	root = AddRoot(cwd);

	loadTree(cwd, root);
	ClearFocusedItem();
	SetItemBold(root);
	SetItemBackgroundColour(root, wxColour(120, 152, 161));
	Expand(root);

	Bind(wxEVT_TREE_ITEM_ACTIVATED, &FileTree::onActivate, this);
	Bind(wxEVT_KILL_FOCUS, &FileTree::onKillFocus, this);
	Bind(wxEVT_LEFT_DOWN, &FileTree::onClick, this);
	Bind(wxEVT_RIGHT_DOWN, &FileTree::onClick, this);
}

Frame *FileTree::getFrame(void) const {
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
	ClearFocusedItem();
	event.Skip();
}

void FileTree::onKillFocus(wxFocusEvent& event) {
	ClearFocusedItem();
	event.Skip();
}

void FileTree::onClick(wxMouseEvent& event) {
	ClearFocusedItem();
	event.Skip();
}

void FileTree::loadTree(const std::string& cwd, wxTreeItemId parent) {
	for (const auto & file : std::filesystem::directory_iterator(cwd)) {
		wxTreeItemId item = AppendItem(parent, file.path().filename().string());
		std::string ext = file.path().extension();
		if (!ext.empty() && ext[0] == '.') {
			ext.erase(0, 1);
		}
		if (std::filesystem::is_directory(file.path())) {
			SetItemBackgroundColour(item, wxColour(120, 152, 161));
			SetItemBold(item);
			loadTree(file.path().string(), item);	
		}
		else {
			SetItemTextColour(item, getColorFromExtension(ext));
		}
	}
}

// every time a file is added or deleted, reload tree
void FileTree::reloadTree(void) {
	DeleteChildren(root);
	loadTree(cwd, root);
	Expand(root);
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

wxColour FileTree::getColorFromExtension(const std::string& ext) const {
	std::hash<std::string> h;
	unsigned int value = h(ext);
	unsigned int r = 255 - (value >> 16 & 0xff);
	unsigned int g = 255 - ((value >> 8) & 0xff);
	unsigned int b = 255 - (value & 0xff);
	r = (255 - r) / 10 * 3 + r;
	g = (255 - g) / 10 * 3 + g;
	b = (255 - b) / 10 * 3 + b;
	return wxColour(r, g, b);
}
