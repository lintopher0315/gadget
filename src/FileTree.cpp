#include "Frame.h"
#include "Window.h"
#include "FileTree.h"
#include "FileHelper.h"

FileTree::FileTree(wxWindow *parent) : wxTreeCtrl(parent, wxID_ANY, wxDefaultPosition, wxSize(wxGetDisplaySize().GetWidth()/4, wxGetDisplaySize().GetHeight()/2), wxTR_DEFAULT_STYLE | wxTR_NO_LINES, wxDefaultValidator, wxTreeCtrlNameStr) {
    wxFont *font = new wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString);
	SetFont(*font);
	SetMinClientSize(wxSize(100, 100));
	SetBackgroundColour(wxColour(37, 37, 38));
	SetIndent(20);

	cwd = getFrame()->cwd;
	root = AddRoot(cwd);

	int numFiles = loadTree(cwd, root);
	SortChildren(root);
	ClearFocusedItem();
	SetItemBold(root);
	SetItemBackgroundColour(root, getDirectoryColor(numFiles));
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
		int lexer = FileHelper::getLexerFromExtension(relPath);
		w->panel->AddPage(new Editor(w->panel, lexer), relPath, true);
		w->getCurrentEditor()->relPath = relPath;
		w->getCurrentEditor()->loadFormatted(absPath);
		w->getCurrentEditor()->saved = true;
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

int FileTree::loadTree(const std::string& cwd, wxTreeItemId parent) {
	int numFiles = 0;
	for (const auto & file : std::filesystem::directory_iterator(cwd)) {
		wxTreeItemId item = AppendItem(parent, file.path().filename().string());
		std::string ext = file.path().extension();
		if (!ext.empty() && ext[0] == '.') {
			ext.erase(0, 1);
		}
		if (std::filesystem::is_directory(file.path())) {
			int num = loadTree(file.path().string(), item);	
			numFiles += num;
			SortChildren(item);
			SetItemBackgroundColour(item, wxColour(getDirectoryColor(num)));
			SetItemBold(item);
		}
		else {
			SetItemTextColour(item, getColorFromExtension(ext));
			++numFiles;
		}
	}
	return numFiles;
}

// every time a file is added or deleted, reload tree
void FileTree::reloadTree(void) {
	DeleteChildren(root);
	int numFiles = loadTree(cwd, root);
	SortChildren(root);
	SetItemBackgroundColour(root, getDirectoryColor(numFiles));
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

wxColour FileTree::getDirectoryColor(const int& num) const {
	if (num == 0) {
		return wxColour(225, 191, 227);
	}
	if (num <= 10) {
		return wxColour(207, 109, 109);
	}
	if (num <= 30) {
		return wxColour(173, 173, 76);
	}
	if (num <= 50) {
		return wxColour(76, 173, 76);
	}
	if (num <= 70) {
		return wxColour(76, 173, 173);
	}
	return wxColour(102, 102, 232);
}

int FileTree::OnCompareItems(const wxTreeItemId& item1, const wxTreeItemId& item2) {
	std::string text1 = std::string(GetItemText(item1).mb_str());
	std::string text2 = std::string(GetItemText(item2).mb_str());
	std::string ext1 = FileHelper::getExtension(text1);
	std::string ext2 = FileHelper::getExtension(text2);

	if (ItemHasChildren(item1) && ItemHasChildren(item2)) {
		return text1.compare(text2);
	}
	else if (ItemHasChildren(item1) && !ItemHasChildren(item2)) {
		return -1;
	}
	else if (!ItemHasChildren(item1) && ItemHasChildren(item2)) {
		return 1;
	}
	
	if (FileHelper::isDotFile(text1) && FileHelper::isDotFile(text2)) {
		return text1.compare(text2);
	}
	else if (FileHelper::isDotFile(text1) && !FileHelper::isDotFile(text2)) {
		return -1;
	}
	else if (!FileHelper::isDotFile(text1) && FileHelper::isDotFile(text2)) {
		return 1;
	}

	if (ext1.empty() && ext2.empty()) {
		return text1.compare(text2);
	}
	else if (ext1.empty() && !ext2.empty()) {
		return -1;
	}
	else if (!ext1.empty() && ext2.empty()) {
		return 1;
	}

	if (ext1 == ext2) {
		return text1.compare(text2);
	}
	return ext1.compare(ext2);
}
