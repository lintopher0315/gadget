#pragma once

#include <wx/wx.h>
#include <wx/stc/stc.h>

class Editor;

#include "Window.h"

class Editor : public wxStyledTextCtrl {
    public:
        Editor(wxWindow *parent);

        Window *getWindow(void);

        int linePos(void) const;
		int currLine(void) const;
        int lineStartPos(void) const;
        int lineEndPos(void) const;
        void caretLeft(const int& num);
        void caretRight(const int& num);
        void caretUp(const int& num);
        void caretDown(const int& num);
        void append(void);
        void insertLineBelow(const int& num);
        void insertLineAbove(const int& num);
		void wordLeft(const int& num);
		void wordRight(const int& num);

        std::string relPath;
		bool saved;

	private:
        void onChar(wxKeyEvent& event);
        void onKey(wxKeyEvent& event);
		void onClick(wxMouseEvent& event);
		void onModified(wxStyledTextEvent& event);
};
