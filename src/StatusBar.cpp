#include "StatusBar.h"

StatusBar::StatusBar(wxWindow *parent) : wxRichTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(wxGetDisplaySize().GetWidth()/2, 30), wxRE_MULTILINE, wxDefaultValidator, "status") {
    SetEditable(false);
    EnableVerticalScrollbar(false);
}
