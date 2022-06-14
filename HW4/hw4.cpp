#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <debugger.hpp>
#include <string>

// Program usage message
void usage(const std::string& program_name);

int main(int argc, char* argv[]) {
    // Parse arguments
    std::string script{}, program_name{};
    char argument{};
    while ((argument = getopt(argc, argv, "s:h")) != -1) {
        switch (argument) {
            case 's': {
                script = optarg;
                break;
            }
            case 'h':
            case '?':
            default: {
                usage(argv[0]);
                return 1;
            }
        }
    }
    if (optind < argc) program_name = argv[optind];

    Debugger debugger{script, program_name};
    debugger.debug();
    return 0;
}

// Program usage message
void usage(const std::string& program_name) {
    std::cout << "Usage: " << program_name << " [options] [program]\n"
              << "Program options:\n"
              << "  -s script  A script representing user inputs\n"
              << "  -h         This message" << std::endl;
}