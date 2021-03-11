#include "Gadget.h"
#include "Frame.h"
#include "res/gadget.xpm"

bool Gadget::OnInit() {
    wxSize s = wxGetDisplaySize();
    s.Scale(0.5, 0.5);

    Frame *f = new Frame(s);
    f->Show(true);

	wxIcon icon(gadget_icon);
	f->SetIcon(icon);

    return true;
}

wxIMPLEMENT_APP(Gadget);
