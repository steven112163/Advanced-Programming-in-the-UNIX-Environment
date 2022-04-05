#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

void set_ld_preload(const std::string& logger_path);
void unset_ld_preload();