#include "StatusBar.h"

StatusBar::StatusBar(wxWindow *parent) : wxRichTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(wxGetDisplaySize().GetWidth()/2, 30)) {
    SetEditable(false);
    SetBackgroundColour(wxColour(0xB5, 0xB5, 0xB5));
    EnableVerticalScrollbar(false);
    wxFont *font = new wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString);
    SetFont(*font);
    AppendText("~ NORMAL ~");
}
