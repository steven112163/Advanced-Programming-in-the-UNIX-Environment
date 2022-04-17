#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

void print_message() {
    std::cout
        << "usage: ./logger [-o file] [-p sopath] [--] cmd [cmd args ...]\n"
        << "        -p: set the path to logger.so, default = ./logger.so\n"
        << "        -o: print output to file, print to \"stderr\" if no file "
           "specified\n"
        << "        --: separate the arguments for logger and for the command\n"
        << std::endl;
}

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
                print_message();
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

    // Fork a process for running the given command
    int status{};
    pid_t pid{fork()};
    if (pid == 0) {
        // Child
        // Set LD_PRELOAD
        char variable[logger_path.length() + 1];
        strcpy(variable, logger_path.c_str());
        putenv(variable);

        // Set OUTPUT_FILE
        if (!output_file.empty()) {
            char variable[12 + output_file.length() + 1];
            strcpy(variable, ("OUTPUT_FILE=" + output_file).c_str());
            putenv(variable);
        }

        // Construct arguments for execvp
        int len_of_arg = arguments.size() + 2;
        char* arg[len_of_arg];
        arg[0] = strdup(command.c_str());
        for (int idx = 1; idx < len_of_arg - 1; idx++)
            arg[idx] = strdup(arguments[idx - 1].c_str());
        arg[len_of_arg - 1] = nullptr;

        // Execute command
        execvp(arg[0], arg);
    } else {
        // Parent
        wait(&status);
    }

    return 0;
}