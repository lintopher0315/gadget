#include "Window.h"

Window::Window(wxWindow *parent) : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize) {
    sizer = new wxBoxSizer(wxVERTICAL);

    panel = new Panel(this);
    statusBar = new StatusBar(this);
    commandBar = new CommandBar(this);

    sizer->Add(panel, 1, wxEXPAND | wxALL, 0);
    sizer->Add(statusBar, 0, wxEXPAND | wxALL, -3);
    sizer->Add(commandBar, 0, wxEXPAND | wxALL, 0);

    SetSizer(sizer);

    command = new Command();

    mode = NORMAL_MODE;
    currEditor = 0;
}

Frame *Window::getFrame() const {
    return (Frame *)GetParent();
}

Editor *Window::getCurrentEditor() const {
    return (Editor *)panel->GetPage(currEditor);
}

void Window::executeNormal(const int& cmdInd) {
    switch(cmdInd) {
        case 0:
			doInsertion();
            break;
        case 1:
			doBasicMovement();
            break;
        case 2:
			doNewLine();
            break;
        case 3:
			doLineJump();
            break;
		case 4:
			doTabChange();
			break;
    }
    command->clear();
    commandBar->Clear();
}

void Window::executeCommand(const int& cmdInd) {
    switch(cmdInd) {
        case 0:
			doQuitFile();
            break;
        case 1:
			doSaveFile();
            break;
        case 2:
			doOpenFile();
            break;
        case 3:
			doNewTab();
            break;
    }
    command->clear();
    commandBar->Clear();
}

void Window::doInsertion() {
    Editor *e = getCurrentEditor();

	mode = EDIT_MODE;

	if (command->cmd == "a") {
		e->append();
	}
	else if (command->cmd == "A") {
		e->LineEnd();
	}
	else if (command->cmd == "I") {
		e->VCHome();
	}
	statusBar->modeDisplay->setCenteredText("~ EDIT ~");
}

void Window::doBasicMovement() {
    std::pair<int, std::string> parsedCmd = command->parseNormal();
    Editor *e = getCurrentEditor();

	if (parsedCmd.second == "h") {
		e->caretLeft(parsedCmd.first);
	}
	else if (parsedCmd.second == "j") {
		e->caretDown(parsedCmd.first);
	}
	else if (parsedCmd.second == "k") {
		e->caretUp(parsedCmd.first);
	}
	else if (parsedCmd.second == "l") {
		e->caretRight(parsedCmd.first);
	}
}

void Window::doNewLine() {
    std::pair<int, std::string> parsedCmd = command->parseNormal();
    Editor *e = getCurrentEditor();

	if (parsedCmd.second == "o") {
		e->insertLineBelow(parsedCmd.first);
	}
	else if (parsedCmd.second == "O") {
		e->insertLineAbove(parsedCmd.first);
	}
}

void Window::doLineJump() {
    Editor *e = getCurrentEditor();

	if (command->cmd == "_") {
		e->VCHome();
	}
	else if (command->cmd == "$") {
		e->LineEnd();
		e->caretLeft(1);
	}
	else if (command->cmd == "0") {
		e->Home();
	}
}

void Window::doTabChange() {
    std::pair<int, std::string> parsedCmd = command->parseNormal();
	panel->setTab(parsedCmd.first);
}

void Window::doQuitFile() {
	// later: check if curr editor saved before exiting
	panel->deleteCurr();
}

void Window::doSaveFile() {
    std::vector<std::string> parsedCmd = command->parseCommand();
    Editor *e = getCurrentEditor();

	if (parsedCmd.size() == 1) {
		if (e->relPath == "") {
			// later this should display an error: editor has no file
			return;
		}
	}
	else {
		if (isValidPath(parsedCmd[1])) {
			e->relPath = parsedCmd[1];
			panel->SetPageText(currEditor, parsedCmd[1]);
		}
		else {
			// also display error
			return;
		}
	}
	if (isValidPath(e->relPath)) {
		if (!isExistingPath(e->relPath)) {
			// reload file tree
			e->SaveFile(getFrame()->cwd + e->relPath);
			getFrame()->tree->reloadTree();
		}
		else {
			e->SaveFile(getFrame()->cwd + e->relPath);
		}
	}
	if (parsedCmd[0] == "wq") {
		panel->deleteCurr();
	}
}

void Window::doOpenFile() {
    std::vector<std::string> parsedCmd = command->parseCommand();
    Editor *e = getCurrentEditor();

    // later: check if curr editor saved before replacing
	if (isValidPath(parsedCmd[1])) {
		e->relPath = parsedCmd[1];
		panel->SetPageText(currEditor, parsedCmd[1]);
		if (isExistingPath(parsedCmd[1])) {
			e->LoadFile(getFrame()->cwd + e->relPath);
		}
		// else indicate that it's a new file
	}
}

void Window::doNewTab() {
    std::vector<std::string> parsedCmd = command->parseCommand();

	if (parsedCmd.size() == 1) {
		panel->AddPage(new Editor(panel), "[NO FILE]", true);
		return;
	}
	for (int i = 1; i < parsedCmd.size(); ++i) {
		if (!isValidPath(parsedCmd[i])) {
			continue;
		}
		panel->AddPage(new Editor(panel), parsedCmd[i], true);
		getCurrentEditor()->relPath = parsedCmd[i];
		if (isExistingPath(parsedCmd[i])) {
			getCurrentEditor()->LoadFile(getFrame()->cwd + getCurrentEditor()->relPath);
		}
	}
}

bool Window::isExistingPath(const std::string& relPath) const {
    if (relPath.empty()) {
        return false;
    }
    std::string absPath = getFrame()->cwd + relPath;
    if (std::filesystem::exists(absPath) && std::filesystem::is_regular_file(absPath)) {
        return true;
    }
    return false;
}

bool Window::isValidPath(const std::string& relPath) const {
    // currently only allows relative paths from cwd
    if (relPath.empty()) {
        return false;
    }
    std::string absDir = getFrame()->cwd + relPath;
    // maybe check for backwards slash on Windows
    while (absDir.back() != '/') {
        absDir.pop_back();
    }
    if (std::filesystem::exists(absDir) && std::filesystem::is_directory(absDir)) {
        return true;
    }
    return false;
}
