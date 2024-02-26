/**
 * shell.cpp
 * Julian Rachele
 * Wrapper for accessing REDCRAFT binaries via the command line
 */
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>

#include <unistd.h>
#include <limits.h>

void usage() {
    std::cout << "Usage: redcraft [-S|--script] <binary|script> [options] [-V|--version]\nSee redcraft --help or the "
				 "REDCRAFT documentation for details\n";
}

std::string INSTALL_PREFIX = std::string(INSTALL_PATH);
std::string VERSION = std::string(INSTALL_VERSION);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        usage();
        return 0;
    }
    std::string PATH = INSTALL_PREFIX + "/redcraft/";
    int offset;
    if (strcmp("-S", argv[1]) == 0 || strcmp("--script", argv[1]) == 0) {
        // execute a redcraft script
        PATH += "/scripts/";
        offset = 2;
        if (argc < 3) {
            usage();
            return 0;
        }
    } else if (strcmp("-H", argv[1]) == 0 || strcmp("--help", argv[1]) == 0) {
        usage();
	std::string cmd = "ls ";
#ifdef _WIN32
	cmd = "dir ";
#endif
	std::string full_path = INSTALL_PREFIX + "/redcraft/";
	std::cout << "REDCRAFT version: " << VERSION << std::endl;
    	std::cout << "\nList of available binaries:" << std::endl;
	system((cmd + "\"" + full_path + "bin" + "\"").c_str());
	std::cout << "\nList of available scripts:" << std::endl;
	system((cmd + "\"" + full_path + "scripts" + "\"").c_str());
	return 0;
    } else if (strcmp("-V", argv[1]) == 0 || strcmp("--version", argv[1]) == 0) {
	std::cout << "REDCRAFT version: " << VERSION << std::endl;
	return 0;
    } else {
        // execute a redcraft binary
        PATH += "/bin/";
        offset = 1;
    }
    std::stringstream soptions;
    std::string options;
    // PATH = "\"" + PATH + "\"";
    soptions << "\"" << PATH << argv[offset];
    for (int i = offset+2; i <= argc; i++) {
        soptions << " \"" << argv[i-1] << "\"";
    }
    soptions << "\"";
    try {
        options = soptions.str();
    } catch (const std::exception &) {
        std::cerr << "Improper formatting\n";
        return -1;
    }
    options = "\"" + options + "\"";
//    std::cout << options << std::endl;
    system(options.c_str());
}

