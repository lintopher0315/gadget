#include "Window.h"

Window::Window(wxWindow *parent) : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize) {
	SetMinClientSize(wxSize(500, 100));

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
	lastCopiedMode = NORMAL_MODE;
}

Frame *Window::getFrame(void) const {
    return (Frame *)GetParent();
}

Editor *Window::getCurrentEditor(void) const {
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
			doIntraLineJump();
            break;
		case 4:
			doInterLineJump();
			break;
		case 5:
			doTabChange();
			break;
		case 6:
			doWordJump();
			break;
		case 7:
			doCharSearch();
			break;
		case 8:
			doVisualMode();
			break;
		case 9:
			doLineMode();
			break;
		case 10:
			doPaste();
			break;
    }
    command->clear();
    commandBar->Clear();
	updateStatus();
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
		case 4:
			doSplitTab();
			break;
    }
    command->clear();
    commandBar->Clear();
	updateStatus();
}

void Window::executeVisual(const int& cmdInd) {
	switch(cmdInd) {
		case 0:
			doBasicVisMovement();
			break;
		case 1:
			doVisOrLineDelete();
			break;
		case 2:
			doVisOrLineCaseChange();
			break;
		case 3:
			doVisInterLineJump();
			break;
		case 4:
			doVisOrLineCopy();
			break;
	}
	command->clear();
	commandBar->Clear();
	updateStatus();
}

void Window::executeLine(const int& cmdInd) {
	switch(cmdInd) {
		case 0:
			doBasicLineMovement();
			break;
		case 1:
			doVisOrLineDelete();
			break;
		case 2:
			doVisOrLineCaseChange();
			break;
		case 3:
			doLineShift();
			break;
		case 4:
			doVisOrLineCopy();
			break;
		case 5:
			doLineInterLineJump();
			break;
	}
	command->clear();
	commandBar->Clear();
	updateStatus();
}

void Window::doInsertion(void) {
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
}

void Window::doBasicMovement(void) {
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

void Window::doNewLine(void) {
    std::pair<int, std::string> parsedCmd = command->parseNormal();
    Editor *e = getCurrentEditor();

	if (parsedCmd.second == "o") {
		e->insertLineBelow(parsedCmd.first);
	}
	else if (parsedCmd.second == "O") {
		e->insertLineAbove(parsedCmd.first);
	}
}

void Window::doIntraLineJump(void) {
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

void Window::doInterLineJump(void) {
	Editor *e = getCurrentEditor();

	if (command->cmd == "gg") {
		e->GotoLine(0);
	}
	else if (command->cmd == "G") {
		e->GotoLine(e->GetLineCount() - 1);
	}
	else {
		std::pair<int, std::string> parsedCmd = command->parseNormal();
		e->GotoLine(parsedCmd.first - 1);
	}
}

void Window::doTabChange(void) {
    std::pair<int, std::string> parsedCmd = command->parseNormal();
	panel->setTab(parsedCmd.first);
}

void Window::doWordJump(void) {
    std::pair<int, std::string> parsedCmd = command->parseNormal();
	Editor *e = getCurrentEditor();

	if (parsedCmd.second == "w") {
		e->wordRight(parsedCmd.first);
	}
	else {
		e->wordLeft(parsedCmd.first);
	}
}

void Window::doCharSearch(void) {
	Editor *e = getCurrentEditor();

	if (command->cmd[0] == 'f') {
		e->charSearchAhead(command->cmd[1], 1);
	}
	else if (command->cmd[0] == 'F') {
		e->charSearchBehind(command->cmd[1], 1);
	}
	else if (command->cmd[0] == 't') {
		e->charSearchAhead(command->cmd[1], 0);
	}
	else if (command->cmd[0] == 'T') {
		e->charSearchBehind(command->cmd[1], 0);
	}
}

void Window::doVisualMode(void) {
	mode = VISUAL_MODE;
}

void Window::doLineMode(void) {
	Editor *e = getCurrentEditor();

	mode = LINE_MODE;

	e->SetAnchor(e->lineStartPos());
	e->SetCurrentPos(e->lineEndPos());
}

void Window::doPaste(void) {
	Editor *e = getCurrentEditor();

	if (lastCopiedMode == NORMAL_MODE) {
		return;
	}
	if (lastCopiedMode == VISUAL_MODE) {
		if (command->cmd == "p") {
			e->CharRight();
		}
		e->Paste();
	}
	else if (lastCopiedMode == LINE_MODE) {
		if (command->cmd == "P") {
			e->Home();
			e->NewLine();
			e->caretUp(1);
			e->Paste();
		}
		else if (command->cmd == "p") {
			e->LineEnd();
			e->NewLine();
			e->Paste();
		}
	}
}

void Window::doQuitFile(void) {
	// later: check if curr editor saved before exiting
	panel->deleteCurr();
}

void Window::doSaveFile(void) {
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
		e->saved = true;
	}
	if (parsedCmd[0] == "wq") {
		panel->deleteCurr();
	}
}

void Window::doOpenFile(void) {
    std::vector<std::string> parsedCmd = command->parseCommand();
    Editor *e = getCurrentEditor();

    // later: check if curr editor saved before replacing
	if (isValidPath(parsedCmd[1])) {
		e->relPath = parsedCmd[1];
		panel->SetPageText(currEditor, parsedCmd[1]);
		if (isExistingPath(parsedCmd[1])) {
			e->LoadFile(getFrame()->cwd + e->relPath);
		}
		e->saved = true;
		// else indicate that it's a new file
	}
}

void Window::doNewTab(void) {
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

void Window::doSplitTab(void) {
	if (command->cmd == "split") {
		panel->Split(currEditor, wxBOTTOM);
	}
	else {
		panel->Split(currEditor, wxRIGHT);
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

void Window::doBasicVisMovement(void) {
    std::pair<int, std::string> parsedCmd = command->parseNormal();
    Editor *e = getCurrentEditor();

	if (parsedCmd.second == "h") {
		e->caretLeftVis(parsedCmd.first);
	}
	else if (parsedCmd.second == "j") {
		e->caretDownVis(parsedCmd.first);
	}
	else if (parsedCmd.second == "k") {
		e->caretUpVis(parsedCmd.first);
	}
	else if (parsedCmd.second == "l") {
		e->caretRightVis(parsedCmd.first);
	}
}

void Window::doVisOrLineDelete(void) {
	Editor *e = getCurrentEditor();

	lastCopiedMode = mode;
	e->cutSelection();
	mode = NORMAL_MODE;
}

void Window::doVisOrLineCaseChange(void) {
	Editor *e = getCurrentEditor();

	if (command->cmd == "u") {
		e->caseChangeSelection(false);
	}
	else if (command->cmd == "U") {
		e->caseChangeSelection(true);
	}

	e->removeSelection();
	mode = NORMAL_MODE;
}

void Window::doVisInterLineJump(void) {
	Editor *e = getCurrentEditor();

	if (command->cmd == "gg") {
		e->jumpStartVis();
	}
	else if (command->cmd == "G") {
		e->jumpEndVis();
	}
	else {
		std::pair<int, std::string> parsedCmd = command->parseNormal();
		e->jumpLineVis(parsedCmd.first - 1);
	}
}

void Window::doVisOrLineCopy(void) {
	Editor *e = getCurrentEditor();

	lastCopiedMode = mode;
	e->copySelection();
	e->removeSelection();
	mode = NORMAL_MODE;
}

void Window::doBasicLineMovement(void) {
	std::pair<int, std::string> parsedCmd = command->parseNormal();
	Editor *e = getCurrentEditor();

	if (parsedCmd.second == "j") {
		e->caretDownLine(parsedCmd.first);
	}
	else if (parsedCmd.second == "k") {
		e->caretUpLine(parsedCmd.first);
	}
}

void Window::doLineShift(void) {
	std::pair<int, std::string> parsedCmd = command->parseNormal();
	Editor *e = getCurrentEditor();

	if (parsedCmd.second == "<") {
		e->shiftLine(parsedCmd.first, 0);
	}
	else if (parsedCmd.second == ">") {
		e->shiftLine(parsedCmd.first, 1);
	}

	e->removeSelection();
	mode = NORMAL_MODE;
}

void Window::doLineInterLineJump(void) {
	Editor *e = getCurrentEditor();

	if (command->cmd == "gg") {
		e->jumpStartLine();
	}
	else if (command->cmd == "G") {
		e->jumpEndLine();
	}
	else {
		std::pair<int, std::string> parsedCmd = command->parseNormal();
		e->jumpLineLine(parsedCmd.first - 1);
	}
}

void Window::updateStatus(void) {
	Editor *e = getCurrentEditor();

	if (mode == NORMAL_MODE) {
		statusBar->modeDisplay->setText(" ~NORMAL~");
		statusBar->modeDisplay->setBackground(wxColour(219, 131, 0));
	}
	else if (mode == EDIT_MODE) {
		statusBar->modeDisplay->setText("  ~EDIT~");
		statusBar->modeDisplay->setBackground(wxColour(120, 161, 109));
	}
	else if (mode == VISUAL_MODE) {
		statusBar->modeDisplay->setText(" ~VISUAL~");
		statusBar->modeDisplay->setBackground(wxColour(147, 196, 82));
	}
	else if (mode == LINE_MODE) {
		statusBar->modeDisplay->setText("  ~LINE~");
		statusBar->modeDisplay->setBackground(wxColour(222, 149, 222));
	}

	if (e->relPath == "") {
		statusBar->pathDisplay->setText("[NO FILE]");
		statusBar->pathDisplay->setForeground(wxColour(227, 11, 11));
		statusBar->pathDisplay->setBackground(wxColour(214, 141, 141));
	}
	else {
		std::string pathText = getFrame()->cwd + e->relPath;
		std::string panelText = std::string(panel->GetPageText(currEditor).mb_str());
		if (!e->saved) {
			pathText += "***";
			statusBar->pathDisplay->setForeground(wxColour(82, 7, 7));
			statusBar->pathDisplay->setBackground(wxColour(214, 141, 141));
			statusBar->pathDisplay->BeginBold();
			statusBar->pathDisplay->setText(pathText);
			statusBar->pathDisplay->EndBold();

			if (panelText[0] != '*') {
				panel->SetPageText(currEditor, "*" + panelText);
			}
		}
		else {
			statusBar->pathDisplay->setForeground(wxColour(82, 7, 7));
			statusBar->pathDisplay->setBackground(wxColour(214, 141, 141));
			statusBar->pathDisplay->setText(pathText);

			if (panelText[0] == '*') {
				panel->SetPageText(currEditor, panelText.substr(1, panelText.size()-1));
			}
		}
	}

	int curr = e->currLine() + 1;
	int len = e->GetLineCount();
	int percent = (int)((double)curr * 100 / len);
	statusBar->positionDisplay->setText(std::to_string(curr) + "," + std::to_string(e->linePos() + 1) + " | " + std::to_string(percent) + "%");
	wxColour colour = statusBar->positionDisplay->getForeground();
	colour.Set(255 - (int)((double)percent * 2.55), 12, 174 - (int)((double)percent * 1.7));
	statusBar->positionDisplay->setForeground(colour);

	statusBar->sizeDisplay->setText(std::to_string(e->GetLength()) + " bytes | " + std::to_string(len) + " lines");
}
