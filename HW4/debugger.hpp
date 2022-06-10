#include <capstone/capstone.h>
#include <sys/ptrace.h>
#include <unistd.h>

#include <cinttypes>
#include <cstdio>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class Debugger {
   public:
    Debugger(const std::string& program_name);
    void debug();

   private:
    // Table for containing all breakpoints
    std::map<int, std::string> breakpoints{};
    // Vector for containing user command split by spaces
    std::vector<std::string> command{};
    // Flag for determining whether we should stop the debugger
    bool stop{false};

    void print_prompt();
    void command_handler();
    void set_a_breakpoint();
    void delete_a_breakpoint();
    void list_all_breakpoints();
    void continue_the_execution();
    void disassemble_instructions();
    void dump_the_memory();
    void quit_from_the_debugger();
    void get_a_register();
    void set_the_register();
    void get_all_registers();
    void help();
    void load_a_program();
    void run_the_program();
    void show_memory_layout();
    void run_a_single_instruction();
    void start_the_program();
};