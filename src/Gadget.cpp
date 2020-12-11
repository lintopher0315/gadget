#include "Gadget.h"
#include "Frame.h"

bool Gadget::OnInit() {
    wxSize s = wxGetDisplaySize();
    s.Scale(0.5, 0.5);

    Frame *f = new Frame(s);
    f->Show(true);
    return true;
}

wxIMPLEMENT_APP(Gadget);
