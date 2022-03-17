#include <dirent.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
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
std::string construct_path_name(const int& pid, const char* field);
void print_spaces(const int count);
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

std::string construct_path_name(const int& pid, const char* field) {
    int path_size = std::snprintf(nullptr, 0, "/proc/%d/%s", pid, field);
    char path_name[path_size + 1];
    std::snprintf(path_name, path_size + 1, "/proc/%d/%s", pid, field);
    return std::string(path_name);
}

void print_spaces(const int count) {
    for (int i = 0; i < count; i++) std::cout << " ";
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
                         const std::string& filename_regex, int pid) {
    std::ifstream ifs{};

    // Open comm to find command
    std::string path_name{construct_path_name(pid, "comm")};
    std::string command{};
    ifs.open(path_name);
    if (ifs.good()) {
        std::getline(ifs, command);
        ifs.close();
    } else {
        ifs.close();
        return;
    }

    // Find matching username
    std::string user{};
    path_name = construct_path_name(pid, "status");
    ifs.open(path_name);
    if (ifs.good()) {
        // Get the line with UID
        for (int i = 0; i < 9; i++) std::getline(ifs, user);
        std::istringstream ss(user);
        ss >> user;

        // Extract UID
        uid_t uid;
        ss >> uid;

        // Get username from UID
        passwd* pws = getpwuid(uid);
        user = pws->pw_name;
        ifs.close();
    } else {
        ifs.close();
        return;
    }

    std::string fd{};
    std::string type{};
    int node{-1};
    std::string name{};

    processes.emplace_back(command, pid, user, fd, type, node, name);
}

void print_results(const std::vector<CommandRecord>& processes) {
    std::cout
        << "COMMAND\t\t\t\t\tPID\t\tUSER\t\t\tFD\t\tTYPE\t\tNODE\t\tNAME\n";
    for (auto& record : processes) {
        std::cout << record.command;
        print_spaces(40 - record.command.length());

        std::cout << record.pid << "\t\t" << record.user;
        print_spaces(24 - record.user.length());
        
        std::cout << 1 << "\t\t" << record.type << "\t\t" << record.node
                  << "\t\t" << record.name << "\t\t" << std::endl;
    }
}