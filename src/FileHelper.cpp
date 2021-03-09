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

int FileHelper::getExtension(const std::string& relPath) {
	int start = relPath.find_last_of('.');
	if (start == std::string::npos) {
		return NULL_LEX;
	}
	std::string ext = relPath.substr(start + 1, relPath.size() - start - 1);
	if (ext == "cpp" || ext == "cc" || ext == "cxx") {
		return CPP_LEX;
	}
	return NULL_LEX;
}
