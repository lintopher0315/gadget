#include "FileHelper.h"

bool FileHelper::isExistingPath(const std::string& cwd, const std::string& relPath) {
    if (relPath.empty()) {
        return false;
    }
    std::string absPath = cwd + relPath;
    if (std::filesystem::exists(absPath) && std::filesystem::is_regular_file(absPath)) {
        return true;
    }
    return false;
}

bool FileHelper::isValidPath(const std::string& cwd, const std::string& relPath) {
    // currently only allows relative paths from cwd
    if (relPath.empty()) {
        return false;
    }
    std::string absDir = cwd + relPath;
    // maybe check for backwards slash on Windows
    while (absDir.back() != '/') {
        absDir.pop_back();
    }
    if (std::filesystem::exists(absDir) && std::filesystem::is_directory(absDir)) {
        return true;
    }
    return false;
}

bool FileHelper::isReadOnlyFile(const std::string& cwd, const std::string& relPath) {
	std::string absPath = cwd + relPath;
	if (FileHelper::isExistingPath(cwd, relPath)) {
		std::filesystem::perms p = std::filesystem::status(absPath).permissions();
		if ((p & std::filesystem::perms::owner_write) == std::filesystem::perms::none) {
			return true;
		}
	}
	return false;
}

bool FileHelper::isDotFile(const std::string& relPath) {
	std::string cp = relPath;
	int dir = cp.find_last_of('/');
	if (dir != std::string::npos) {
		cp = cp.substr(dir + 1, cp.size() - dir - 1);	
	}
	int dot = cp.find_last_of('.');
	if (dot == std::string::npos) {
		return false;
	}
	if (dot == 0) {
		return true;
	}
	return false;
}

std::string FileHelper::getExtension(const std::string& relPath) {
	int start = relPath.find_last_of('.');
	if (start == std::string::npos) {
		return "";
	}
	if (FileHelper::isDotFile(relPath)) {
		return "";
	}
	return relPath.substr(start + 1, relPath.size() - start - 1);
}

int FileHelper::getLexerFromExtension(const std::string& relPath) {
	std::string ext = getExtension(relPath);
	if (ext.empty()) {
		return NULL_LEX;
	}
	if (ext == "cpp" || ext == "cc" || ext == "cxx" || ext == "h" || ext == "hpp") {
		return CPP_LEX;
	}
	return NULL_LEX;
}
