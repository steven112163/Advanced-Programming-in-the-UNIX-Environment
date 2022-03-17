#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

struct CommandRecord {
    std::string command{};
    int pid{};
    std::string user{};
    std::string fd{};
    std::string type{};
    int node{-1};
    std::string name{};

    CommandRecord(std::string _command, int _pid, std::string _user,
                  std::string _fd, std::string _type, int _node,
                  std::string _name)
        : command(_command),
          pid(_pid),
          user(_user),
          fd(_fd),
          type(_type),
          node(_node),
          name(_name) {}
};

void usage(const char* program_name);
bool isnumber(const std::string& dir_name);
void find_processes(std::vector<CommandRecord>& processes,
                    const std::string& command_regex,
                    const std::string& type_filter,
                    const std::string& filename_regex);
void collect_information(std::vector<CommandRecord>& processes,
                         const std::string& command_regex,
                         const std::string& type_filter,
                         const std::string& filename_regex, int pid);
void print_results(const std::vector<CommandRecord>& processes);

int main(int argc, char** argv) {
    std::string command_regex{}, type_filter{}, filename_regex{};
    const std::vector<std::string> valid_types{"REG",  "CHR",  "DIR",
                                               "FIFO", "SOCK", "unknown"};

    // Parse arguments
    int opt;
    while ((opt = getopt(argc, argv, "c:t:f:h")) != EOF) {
        switch (opt) {
            case 'c': {
                command_regex = optarg;
                break;
            }
            case 't': {
                auto it = find(valid_types.begin(), valid_types.end(),
                               std::string(optarg));
                if (it != valid_types.end()) {
                    type_filter = optarg;
                    break;
                } else {
                    std::cerr << "Invalid TYPE option" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            case 'f': {
                filename_regex = optarg;
                break;
            }
            case 'h':
            default: {
                usage(argv[0]);
                return 1;
            }
        }
    }

    // Find all processes and collect their information
    std::vector<CommandRecord> processes{};
    find_processes(processes, command_regex, type_filter, filename_regex);

    // Display the results
    print_results(processes);

    return 0;
}

void usage(const char* program_name) {
    std::cout
        << "Usage: " << program_name << " [options]\n"
        << "Program options:\n"
        << "  -c REGEX  A regular expression filter for filtering command "
        << "line\n"
        << "  -t TYPE   A TYPE filter. Valid TYPE includes 'REG', 'CHR', "
        << "'DIR', 'FIFO', 'SOCK', and 'unknown'\n"
        << "  -f REGEX  A regular expression filter for filtering filenames\n"
        << "  -h        This message" << std::endl;
}

bool isnumber(const std::string& dir_name) {
    return !dir_name.empty() &&
           std::all_of(dir_name.begin(), dir_name.end(), ::isdigit);
}

void find_processes(std::vector<CommandRecord>& processes,
                    const std::string& command_regex,
                    const std::string& type_filter,
                    const std::string& filename_regex) {
    // Open /proc directory
    DIR* directory{};
    if (!(directory = opendir("/proc"))) {
        std::cerr << "Couldn't open /proc, error number: " << errno
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    // Iterate all PIDs
    dirent* dir_info{};
    while ((dir_info = readdir(directory)))
        if (isnumber(dir_info->d_name))
            collect_information(processes, command_regex, type_filter,
                                filename_regex, atoi(dir_info->d_name));
    closedir(directory);
}

void collect_information(std::vector<CommandRecord>& processes,
                         const std::string& command_regex,
                         const std::string& type_filter,
                         const std::string& filename_regex, int pid) {}

void print_results(const std::vector<CommandRecord>& processes) {
    std::cout << "COMMAND\t\t\tPID\t\tUSER\t\tFD\t\tTYPE\t\tNODE\t\tNAME\n";
}