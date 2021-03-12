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
		static bool isDotFile(const std::string& relPath);
		static std::string getExtension(const std::string& relPath);
		static int getLexerFromExtension(const std::string& relPath);
};
