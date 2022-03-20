#include <dirent.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_set>
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

// Program usage message
void usage(const char* program_name);

// Check whether the directory name only contains digits
bool isnumber(const std::string& dir_name);

// Construct path name /proc/<pid>/<field>
std::string construct_path_name(const int& pid, const char* field);

// Print spaces in the results for formatting
void print_spaces(const int count);

// Find all processes in /proc
void find_processes(std::vector<CommandRecord>& processes,
                    const std::regex& command_regex,
                    const std::string& type_filter,
                    const std::regex& filename_regex);

// Collect information of each process in the /proc
void collect_information(std::vector<CommandRecord>& processes,
                         const std::regex& command_regex,
                         const std::string& type_filter,
                         const std::regex& filename_regex, const int& pid);

// Find the executable filename of the running process
std::string find_command(const int& pid);

// Find the username who runs the process
std::string find_username(const int& pid);

// Resolve file type
std::string resolve_file_type(const struct stat& stat_buf);

// Resolve symbolic link for current working directory, root directory, and
// program file
bool resolve_sym_link(const int& pid, const char* filename,
                      const std::string& command, const std::string& user,
                      std::vector<CommandRecord>& temp_processes,
                      const std::string& type_filter,
                      const std::regex& filename_regex);

// Find current working directory using function "resolve_sym_link"
bool find_cwd(const int& pid, const std::string& command,
              const std::string& user,
              std::vector<CommandRecord>& temp_processes,
              const std::string& type_filter, const std::regex& filename_regex);

// Find root directory using function "resolve_sym_link"
bool find_rtd(const int& pid, const std::string& command,
              const std::string& user,
              std::vector<CommandRecord>& temp_processes,
              const std::string& type_filter, const std::regex& filename_regex);

// Find program file using function "resolve_sym_link"
bool find_txt(const int& pid, const std::string& command,
              const std::string& user,
              std::vector<CommandRecord>& temp_processes,
              const std::string& type_filter, const std::regex& filename_regex);

// Find memory mapping information
bool find_mem_del(const int& pid, const std::string& command,
                  const std::string& user,
                  std::vector<CommandRecord>& temp_processes,
                  const std::string& type_filter,
                  const std::regex& filename_regex);

// Print information of all running processes
void print_results(const std::vector<CommandRecord>& processes);