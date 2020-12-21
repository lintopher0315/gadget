#include "CommandBar.h"

CommandBar::CommandBar(wxWindow *parent) : wxRichTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(wxGetDisplaySize().GetWidth()/2, 30), wxRE_MULTILINE, wxDefaultValidator) {
    SetEditable(false);
    EnableVerticalScrollbar(false);
    wxFont *font = new wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString);
    SetFont(*font);
}
