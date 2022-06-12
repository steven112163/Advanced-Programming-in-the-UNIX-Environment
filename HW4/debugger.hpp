#include <capstone/capstone.h>
#include <elf.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cinttypes>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

enum State { init, loaded, running };

struct Breakpoint {
    unsigned long id;
    unsigned long long address;
    unsigned char original_instruction;
};

std::vector<std::string> tokenize(const std::string& sentence);

// Class representing a Simplified Scriptable Instruction Level Debugger
// Example:
//      Debugger debugger{};
//      debugger.debug();
//      or
//      Debugger debugger{program_name};
//      debugger.debug();
class Debugger {
   public:
    Debugger();
    Debugger(const std::string& program_name);

    // Function for starting the debugger
    void debug();

   private:
    // Counter for representing the current breakpoint ID
    unsigned long breakpoint_id{0};

    // Table for mapping the ID to the corresponding breakpoint
    std::map<unsigned int, Breakpoint> id_to_breakpoint{};

    // Table for mapping the address to the corresponding breakpoint
    std::unordered_map<unsigned long long, Breakpoint> address_to_breakpoint{};

    // Vector for containing user command split by spaces
    std::vector<std::string> command{};

    // Table for containing all handled commands
    std::unordered_map<std::string, std::function<void()>> handlers{};

    // Struct for containing the tracee's registers
    struct user_regs_struct regs {};

    // Variable for holding the entry address
    unsigned long long int entry_address{};

    // Variable for holding the end address
    unsigned long long int end_address{};

    // Flag for determining whether we should stop the debugger
    bool stop{false};

    // State for representing the debugger state
    State state{State::init};

    // PID for indicating the forked child
    pid_t child{};

    // Flag for indicating the status of the tracee
    int wait_status{};

    // String for indicating the current program
    std::string program_name{};

    // Function for setting up the handler table
    void setup_handler_table();

    // Function for checking whether the tracee is stopped
    bool is_stopped();

    // Function for checking whether current state is running
    bool is_running();

    // Function for printing sdb> prompt
    void print_prompt();

    // Function for reading user inputs and send the command to the responsible
    // handler
    void command_handler();

    // Function for setting a breakpoint at the user-typed address
    void set_a_breakpoint();

    // Function for deleting the breakpoint at the user-typed address
    void delete_a_breakpoint();

    // Function for listing all breakpoints set by the user
    void list_all_breakpoints();

    // Function for checking whether the tracee reaches a breakpoint when it's
    // stopped
    void check_breakpoint();

    // Function for continuing the program execution
    void continue_the_execution();

    // Function for disassembling 10 instructions at the user-typed address
    void disassemble_instructions();

    // Function for dumping 80 bytes of the memory content from the program
    void dump_the_memory();

    // Function for cleaning up the debugger and exiting
    void quit_from_the_debugger();

    // Function for getting the value of a register from the program
    void get_a_register();

    // Function for setting the value  a register in the program
    void set_a_register();

    // Function for getting values of all registers from the program
    void get_all_registers();

    // Function for fetching all registers and providing them to
    // "get_a_register" and "get_all_registers"
    std::unordered_map<std::string, unsigned long long int*> fetch_registers();

    // Function for printing the help message
    void help();

    // Function for loading the user-typed program and executing it
    void load_a_program();

    // Function for running the program
    void run_the_program();

    // Function for showing the memory layout of the running program
    void show_memory_layout();

    // Function for running a single instruction and stepping into the function
    // call
    void run_a_single_instruction();

    // Function for starting the program and stopping at the first instruction
    void start_the_program();
};