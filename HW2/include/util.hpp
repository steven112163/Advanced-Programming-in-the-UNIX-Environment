#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <linux_cmd.hpp>
#include <string>

int get_fd();
void close_fd();
void print_log(const std::string& log);