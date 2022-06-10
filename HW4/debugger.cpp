#include <debugger.hpp>

Debugger::Debugger(const std::string& program_name) {}

void Debugger::debug() {
    while (true) {
        if (this->stop) break;
        this->print_prompt();
        this->command_handler();
    }
}

void Debugger::print_prompt() { std::cout << "sdb> "; }

void Debugger::command_handler() {
    // Get raw command from the user
    std::string raw_command{};
    std::getline(std::cin, raw_command);
    std::stringstream ss(raw_command);

    // Split the raw command by spaces
    std::string word{};
    while (ss >> word) {
        this->command.push_back(word);
    };
    if (this->command.size() == 0) return;

    // Call the target handler based on the user command
    if (this->command[0] == "break" || this->command[0] == "b")
        this->set_a_breakpoint();
    else if (this->command[0] == "delete")
        this->delete_a_breakpoint();
    else if (this->command[0] == "list" || this->command[0] == "l")
        this->list_all_breakpoints();
    else if (this->command[0] == "cont" || this->command[0] == "c")
        this->continue_the_execution();
    else if (this->command[0] == "disasm" || this->command[0] == "d")
        this->disassemble_instructions();
    else if (this->command[0] == "dump" || this->command[0] == "x")
        this->dump_the_memory();
    else if (this->command[0] == "exit" || this->command[0] == "q")
        this->quit_from_the_debugger();
    else if (this->command[0] == "get" || this->command[0] == "g")
        this->get_a_register();
    else if (this->command[0] == "set" || this->command[0] == "s")
        this->set_the_register();
    else if (this->command[0] == "getregs")
        this->get_all_registers();
    else if (this->command[0] == "help" || this->command[0] == "h")
        this->help();
    else if (this->command[0] == "load")
        this->load_a_program();
    else if (this->command[0] == "run" || this->command[0] == "r")
        this->run_the_program();
    else if (this->command[0] == "vmmap" || this->command[0] == "m")
        this->show_memory_layout();
    else if (this->command[0] == "si")
        this->run_a_single_instruction();
    else if (this->command[0] == "start")
        this->start_the_program();
    else
        std::cout << "** Unknown command: " << this->command[0] << std::endl;

    // Clear the command
    this->command.clear();
}

void Debugger::set_a_breakpoint() {}

void Debugger::delete_a_breakpoint() {}

void Debugger::list_all_breakpoints() {}

void Debugger::continue_the_execution() {}

void Debugger::disassemble_instructions() {}

void Debugger::dump_the_memory() {}

void Debugger::quit_from_the_debugger() { this->stop = true; }

void Debugger::get_a_register() {}

void Debugger::set_the_register() {}

void Debugger::get_all_registers() {}

void Debugger::help() {
    std::cout << "- break {instruction-address}: add a break point\n"
              << "- cont: continue execution\n"
              << "- delete {break-point-id}: remove a break point\n"
              << "- disasm addr: disassemble instructions in a file or a "
                 "memory region\n"
              << "- dump addr: dump memory content\n"
              << "- exit: terminate the debugger\n"
              << "- get reg: get a single value from a register\n"
              << "- getregs: show registers\n"
              << "- help: show this message\n"
              << "- list: list break points\n"
              << "- load {path/to/a/program}: load a program\n"
              << "- run: run the program\n"
              << "- vmmap: show memory layout\n"
              << "- set reg val: get a single value to a register\n"
              << "- si: step into instruction\n"
              << "- start: start the program and stop at the first instruction"
              << std::endl;
}

void Debugger::load_a_program() {}

void Debugger::run_the_program() {}

void Debugger::show_memory_layout() {}

void Debugger::run_a_single_instruction() {}

void Debugger::start_the_program() {}