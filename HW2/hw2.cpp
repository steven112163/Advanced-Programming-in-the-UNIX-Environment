#include "hw2.hpp"

int main(int argc, char* argv[]) {
    // Parse arguments
    char ch{};
    std::string output_file{};
    std::string logger_path{"LD_PRELOAD=./logger.so"};
    while ((ch = getopt(argc, argv, "o:p:")) != -1) {
        switch (ch) {
            case 'o': {
                output_file = std::string(optarg);
                break;
            }
            case 'p': {
                logger_path = std::string("LD_PRELOAD=") + std::string(optarg);
                break;
            }
            default: {
                std::cout << "usage: ./logger [-o file] [-p sopath] [--] cmd "
                             "[cmd args ...]\n"
                          << "        -p: set the path to logger.so, default = "
                             "./logger.so\n"
                          << "        -o: print output to file, print to "
                             "\"stderr\" if no file specified\n"
                          << "        --: separate the arguments for logger "
                             "and for the command\n"
                          << std::endl;
                return 1;
            }
        }
    }

    if (optind == argc) {
        std::cout << "no command given\n" << std::endl;
        return 1;
    }

    // Parse command with its arguments
    std::string command{argv[optind]};
    std::vector<std::string> arguments{};
    for (int idx{optind + 1}; idx < argc; idx++)
        arguments.emplace_back(argv[idx]);

    // Set LD_PRELOAD
    set_ld_preload(logger_path);

    // Unset LD_PRELOAD
    unset_ld_preload();

    return 0;
}

void set_ld_preload(const std::string& logger_path) {
    char variable[11 + logger_path.length()];
    strcpy(variable, logger_path.c_str());
    putenv(variable);
}

void unset_ld_preload() { unsetenv("LD_PRELOAD"); }