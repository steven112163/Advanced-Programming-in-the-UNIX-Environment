#include "hw1.hpp"

int main(int argc, char** argv) {
    std::regex command_regex{".*"};
    std::string type_filter{};
    std::regex filename_regex{".*"};
    const std::vector<std::string> valid_types{"REG",  "CHR",  "DIR",
                                               "FIFO", "SOCK", "unknown"};

    // Parse arguments
    int opt;
    while ((opt = getopt(argc, argv, "c:t:f:h")) != EOF) {
        switch (opt) {
            case 'c': {
                command_regex = ".*" + std::string(optarg) + ".*";
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
                filename_regex = ".*" + std::string(optarg) + ".*";
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

// Program usage message
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

// Check whether the directory name only contains digits
bool isnumber(const std::string& dir_name) {
    return !dir_name.empty() &&
           std::all_of(dir_name.begin(), dir_name.end(), ::isdigit);
}

// Construct path name /proc/<pid>/<field>
std::string construct_path_name(const int& pid, const char* field) {
    int path_size = std::snprintf(nullptr, 0, "/proc/%d/%s", pid, field);
    char path_name[path_size + 1];
    std::snprintf(path_name, path_size + 1, "/proc/%d/%s", pid, field);
    return std::string(path_name);
}

// Print spaces in the results for formatting
void print_spaces(const int count) {
    for (int i = 0; i < count; i++) std::cout << " ";
}

// Find all processes in /proc
void find_processes(std::vector<CommandRecord>& processes,
                    const std::regex& command_regex,
                    const std::string& type_filter,
                    const std::regex& filename_regex) {
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

// Collect information of each process in the /proc
void collect_information(std::vector<CommandRecord>& processes,
                         const std::regex& command_regex,
                         const std::string& type_filter,
                         const std::regex& filename_regex, const int& pid) {
    // Temporary storage
    std::vector<CommandRecord> temp_processes{};

    // Find command
    std::string command{find_command(pid)};
    if (command.length() == 0 or !std::regex_match(command, command_regex))
        return;

    // Find matching user
    std::string user{find_username(pid)};
    if (user.length() == 0) return;

    // Find current working directory
    if (!find_cwd(pid, command, user, temp_processes, type_filter)) return;

    // Find root directory
    if (!find_rtd(pid, command, user, temp_processes, type_filter)) return;

    // Find program file
    if (!find_txt(pid, command, user, temp_processes, type_filter)) return;

    processes.insert(processes.end(), temp_processes.begin(),
                     temp_processes.end());
}

// Find the executable filename of the running process
std::string find_command(const int& pid) {
    // Open comm to find command
    std::ifstream ifs{};
    std::string path_name{construct_path_name(pid, "comm")};
    std::string command{};

    ifs.open(path_name);
    if (ifs.good()) std::getline(ifs, command);
    ifs.close();

    return command;
}

// Find the username who runs the process
std::string find_username(const int& pid) {
    // Open status to find UID for username
    std::ifstream ifs{};
    std::string path_name{construct_path_name(pid, "status")};
    std::string username{};

    ifs.open(path_name);
    if (ifs.good()) {
        // Get the line with UID
        std::string line{};
        for (int i = 0; i < 9; i++) std::getline(ifs, line);
        std::istringstream ss(line);
        ss >> line;

        // Extract UID
        uid_t uid;
        ss >> uid;

        // Get username from UID
        passwd* pws = getpwuid(uid);
        username = pws->pw_name;
    }
    ifs.close();

    return username;
}

// Resolve symbolic link for current working directory, root directory, and
// program file
bool resolve_sym_link(const int& pid, const char* filename,
                      const std::string& command, const std::string& user,
                      std::vector<CommandRecord>& temp_processes,
                      const std::string& type_filter) {
    // Read filename to resolve the symbolic link
    std::string path_name{construct_path_name(pid, filename)};
    std::string fd{filename};
    if (fd == "root")
        fd = "rtd";
    else if (fd == "exe")
        fd = "txt";
    std::string type{"unknown"};
    int node{-1};
    std::string name{path_name + " (Permission denied)"};

    char buf[1000];
    ssize_t size;
    if ((size = readlink(path_name.c_str(), buf, 1000)) == -1) {
        if (errno == EACCES) {
            if (type_filter.length() == 0 or type_filter == type)
                temp_processes.emplace_back(command, pid, user, fd, type, node,
                                            name);
        } else
            return false;
    } else {
        struct stat stat_buf;
        if (stat(path_name.c_str(), &stat_buf) == -1) {
            std::cout << errno << std::endl;
            return false;
        }

        if (fd == "txt")
            type = "REG";
        else
            type = "DIR";
        buf[size] = '\0';
        name = buf;
        node = stat_buf.st_ino;
        if (type_filter.length() == 0 or type_filter == type)
            temp_processes.emplace_back(command, pid, user, fd, type, node,
                                        name);
    }

    return true;
}

// Find current working directory using function "resolve_sym_link"
bool find_cwd(const int& pid, const std::string& command,
              const std::string& user,
              std::vector<CommandRecord>& temp_processes,
              const std::string& type_filter) {
    return resolve_sym_link(pid, "cwd", command, user, temp_processes,
                            type_filter);
}

// Find root directory using function "resolve_sym_link"
bool find_rtd(const int& pid, const std::string& command,
              const std::string& user,
              std::vector<CommandRecord>& temp_processes,
              const std::string& type_filter) {
    return resolve_sym_link(pid, "root", command, user, temp_processes,
                            type_filter);
}

// Find program file using function "resolve_sym_link"
bool find_txt(const int& pid, const std::string& command,
              const std::string& user,
              std::vector<CommandRecord>& temp_processes,
              const std::string& type_filter) {
    return resolve_sym_link(pid, "exe", command, user, temp_processes,
                            type_filter);
}

// Print information of all running processes
void print_results(const std::vector<CommandRecord>& processes) {
    std::cout
        << "COMMAND\t\t\t\t\tPID\t\tUSER\t\t\tFD\tTYPE\t\tNODE\t\tNAME\n";
    for (auto& record : processes) {
        std::cout << record.command;
        print_spaces(40 - record.command.length());

        std::cout << record.pid << "\t\t" << record.user;
        print_spaces(24 - record.user.length());

        std::cout << record.fd << "\t" << record.type << "\t\t";

        if (record.node != -1) {
            std::cout << record.node;
            print_spaces(16 - std::to_string(record.node).length());
        } else {
            print_spaces(16);
        }

        std::cout << record.name << "\t\t" << std::endl;
    }
}