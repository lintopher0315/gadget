#include "FileTree.h"

FileTree::FileTree(wxWindow *parent) : wxTreeCtrl(parent, wxID_ANY, wxDefaultPosition, wxSize(wxGetDisplaySize().GetWidth()/4, wxGetDisplaySize().GetHeight()/2), wxTR_DEFAULT_STYLE, wxDefaultValidator, wxTreeCtrlNameStr) {
		
}
