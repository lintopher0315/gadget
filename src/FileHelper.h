#pragma once

#include <string>
#include <filesystem>
#include <iostream>

#define NULL_LEX 0
#define CPP_LEX 1

class FileHelper {
	public:
        static bool isExistingPath(const std::string& cwd, const std::string& relPath);
        static bool isValidPath(const std::string& cwd, const std::string& relPath);
		static bool isReadOnlyFile(const std::string& cwd, const std::string& relPath);
		static int getExtension(const std::string& relPath);
};
