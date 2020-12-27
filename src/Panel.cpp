#include "Panel.h"

Panel::Panel(wxWindow *parent) : wxAuiNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE) {
    AddPage(new Editor(this), "caption");
}
