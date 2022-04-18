#include <unistd.h>

#include <cctype>
#include <cerrno>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <linux_cmd.hpp>
#include <string>

int get_fd();
void close_fd();
void print_log(const std::string& log);
std::string convert_char_buf(const char* ptr);
std::string resolve_fd(const int& fd);
std::string resolve_file(FILE* stream);