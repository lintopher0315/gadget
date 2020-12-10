#include "Gadget.h"
#include "Frame.h"

bool Gadget::OnInit() {
    Frame *f = new Frame();
    f->Show(true);
    return true;
}

wxIMPLEMENT_APP(Gadget);
