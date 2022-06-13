#include <debugger.hpp>

std::vector<std::string> tokenize(const std::string& sentence) {
    std::vector<std::string> tokens{};
    std::stringstream ss(sentence);
    std::string word{};
    while (ss >> word) tokens.push_back(word);

    return tokens;
}

Debugger::Debugger() { this->setup_handler_table(); }

Debugger::Debugger(const std::string& program_name) {
    this->setup_handler_table();

    if (!program_name.empty()) {
        this->command = {"load", program_name};
        this->handlers["load"]();
    }
}

// Function for starting the debugger
void Debugger::debug() {
    while (true) {
        if (this->stop) break;
        this->print_prompt();
        this->command_handler();
    }
}

// Function for setting up the handler table
void Debugger::setup_handler_table() {
    // Break
    this->handlers["break"] = this->handlers["b"] =
        std::bind(&Debugger::set_a_breakpoint, this);

    // Delete
    this->handlers["delete"] = std::bind(&Debugger::delete_a_breakpoint, this);

    // List
    this->handlers["list"] = this->handlers["l"] =
        std::bind(&Debugger::list_all_breakpoints, this);

    // Cont
    this->handlers["cont"] = this->handlers["c"] =
        std::bind(&Debugger::continue_the_execution, this);

    // Disasm
    this->handlers["disasm"] = this->handlers["d"] =
        std::bind(&Debugger::disassemble_instructions, this);

    // Dump
    this->handlers["dump"] = this->handlers["x"] =
        std::bind(&Debugger::dump_the_memory, this);

    // Exit
    this->handlers["exit"] = this->handlers["q"] =
        std::bind(&Debugger::quit_from_the_debugger, this);

    // Get
    this->handlers["get"] = this->handlers["g"] =
        std::bind(&Debugger::get_a_register, this);

    // Set
    this->handlers["set"] = this->handlers["s"] =
        std::bind(&Debugger::set_a_register, this);

    // Getregs
    this->handlers["getregs"] = std::bind(&Debugger::get_all_registers, this);

    // Help
    this->handlers["help"] = this->handlers["h"] =
        std::bind(&Debugger::help, this);

    // Load
    this->handlers["load"] = std::bind(&Debugger::load_a_program, this);

    // Run
    this->handlers["run"] = this->handlers["r"] =
        std::bind(&Debugger::run_the_program, this);

    // Vmmap
    this->handlers["vmmap"] = this->handlers["m"] =
        std::bind(&Debugger::show_memory_layout, this);

    // Si
    this->handlers["si"] = std::bind(&Debugger::run_a_single_instruction, this);

    // Start
    this->handlers["start"] = std::bind(&Debugger::start_the_program, this);
}

// Function for checking whether the tracee is stopped
bool Debugger::is_stopped() {
    if (!WIFSTOPPED(this->wait_status)) {
        std::cerr << "** Tracee is not stopped!" << std::endl;
        this->quit_from_the_debugger();
        return false;
    }
    return true;
}

// Function for checking whether current state is running
bool Debugger::is_running() {
    if (this->state == State::init) {
        std::cout << "** The program is not loaded." << std::endl;
        return false;
    }

    if (this->state == State::loaded) {
        std::cout << "** The program \"" << this->program_name
                  << "\" is not running." << std::endl;
        return false;
    }

    return true;
}

// Function for printing sdb> prompt
void Debugger::print_prompt() { std::cout << "sdb> "; }

// Function for reading user inputs and send the command to the responsible
// handler
void Debugger::command_handler() {
    // Get the command from the user
    std::string raw_command{};
    std::getline(std::cin, raw_command);
    this->command = tokenize(raw_command);
    if (this->command.size() == 0) return;

    // Call the target handler based on the user command
    auto it = this->handlers.find(this->command[0]);
    if (it != this->handlers.end())
        it->second();
    else
        std::cout << "** Unknown command: \"" << this->command[0]
                  << "\". Try \"help\"" << std::endl;
}

// Function for setting a breakpoint at the user-typed address
void Debugger::set_a_breakpoint() {
    if (!this->is_stopped()) return;

    if (!this->is_running()) return;

    if (this->command.size() < 2) {
        std::cout << "** No address is given." << std::endl;
        return;
    }

    ull address{this->to_ull(this->command[1])};

    if (!(this->entry_address < address && address <= this->end_address)) {
        std::cout << "** The given address is invalid." << std::endl;
        return;
    }

    // Record the original instruction and change the byte at the given address
    // to 0xcc
    long mem = ptrace(PTRACE_PEEKTEXT, this->child, address, 0);
    Breakpoint breakpoint{this->breakpoint_id++, address, (unsigned char)mem};
    this->address_to_breakpoint[breakpoint.address] =
        this->id_to_breakpoint[breakpoint.id] = breakpoint;

    ptrace(PTRACE_POKETEXT, this->child, address,
           (mem & 0xffffffffffffff00) | 0xcc);
}

// Function for deleting the breakpoint at the user-typed address
void Debugger::delete_a_breakpoint() {
    if (!this->is_stopped()) return;

    if (!this->is_running()) return;

    if (this->command.size() < 2) {
        std::cout << "** No breakpoint ID is given." << std::endl;
        return;
    }

    ull id{this->to_ull(this->command[1])};

    // Check if the given ID exists
    auto it{this->id_to_breakpoint.find(id)};
    if (it == this->id_to_breakpoint.end()) {
        std::cout << "** No such breakpoint ID." << std::endl;
        return;
    }

    // Change the instruction back to its original byte
    Breakpoint breakpoint{it->second};
    long mem = ptrace(PTRACE_PEEKTEXT, this->child, breakpoint.address, 0);
    ptrace(PTRACE_POKETEXT, this->child, breakpoint.address,
           (mem & 0xffffffffffffff00) | breakpoint.original_instruction);
    this->id_to_breakpoint.erase(id);

    std::cout << "** Breakpoint " << id << " deleted." << std::endl;
}

// Function for listing all breakpoints set by the user
void Debugger::list_all_breakpoints() {
    for (const auto& [id, breakpoint] : this->id_to_breakpoint) {
        std::cout << std::setw(3) << id << ": " << std::setw(8) << std::hex
                  << breakpoint.address << std::endl;

        // Undo io manipulation
        std::cout.copyfmt(std::ios(NULL));
    }
}

// Function for checking whether the tracee reaches a breakpoint when it's
// stopped
void Debugger::check_breakpoint() {
    if (WIFSTOPPED(this->wait_status)) {
        if (WSTOPSIG(this->wait_status) != SIGTRAP) {
            std::cout << "** Child process stopped by a signal (code "
                      << WSTOPSIG(this->wait_status) << ")" << std::endl;
            this->quit_from_the_debugger();
            return;
        }

        // Check if the tracee is stopped by 0xcc
        std::unordered_map<std::string, ull*> registers{
            this->fetch_registers()};
        long mem =
            ptrace(PTRACE_PEEKTEXT, this->child, *(registers["rip"]) - 1, 0);
        unsigned char* inst = (unsigned char*)&mem;
        if (inst[0] == 0xcc) {
            // Return if the interruption is not set by the user
            auto it = this->address_to_breakpoint.find(*(registers["rip"]) - 1);
            if (it == this->address_to_breakpoint.end()) return;

            // Decrement the register RIP and write it back
            *(registers["rip"]) -= 1;
            ptrace(PTRACE_SETREGS, this->child, 0, &this->regs);

            // Change the retrieved breakpoint back to its original instruction
            inst[0] = (it->second).original_instruction;

            // Print the original stopped instruction
            std::cout << "** Breakpoint @";

            csh handle;
            if (cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK) {
                perror("** Failed opening capstone");
                return;
            }
            cs_insn* insn;

            size_t count = cs_disasm(handle, (uint8_t*)&mem, sizeof(mem),
                                     *(registers["rip"]), 1, &insn);
            if (count > 0) {
                std::cout << std::setw(12) << std::right << std::hex
                          << insn[0].address << ":";
                std::stringstream ss;
                for (int i = 0; i < insn[0].size; ++i) {
                    ss << " " << std::setw(2) << std::right << std::setfill('0')
                       << std::hex << (unsigned int)insn[0].bytes[i];
                }

                std::cout << std::left << ss.str() << "\t\t\t"
                          << insn[0].mnemonic << "\t" << insn[0].op_str
                          << std::endl;

                // Undo io manipulation
                std::cout.copyfmt(std::ios(NULL));
            }
            cs_free(insn, count);
            cs_close(&handle);
        }
    }

    if (WIFEXITED(this->wait_status)) {
        std::cout << "** Child process " << this->child
                  << " terminiated normally (code "
                  << WEXITSTATUS(this->wait_status) << ")" << std::endl;
    }
}

// Function for running the stopped instruction and write 0xcc back to the
// breakpoint
void Debugger::run_breakpoint() {
    if (!this->is_stopped()) return;

    // Get the stopped instruction
    std::unordered_map<std::string, ull*> registers{this->fetch_registers()};
    long mem = ptrace(PTRACE_PEEKTEXT, this->child, *(registers["rip"]), 0);
    unsigned char* inst = (unsigned char*)&mem;

    if (inst[0] == 0xcc) {
        // Return if the interruption is not set by the user
        auto it = this->address_to_breakpoint.find(*(registers["rip"]));
        if (it == this->address_to_breakpoint.end()) return;

        // Change the breakpoint back to its original instruction
        inst[0] = (it->second).original_instruction;
        ptrace(PTRACE_POKETEXT, this->child, *(registers["rip"]), mem);

        // Run the stopped instruction
        ptrace(PTRACE_SINGLESTEP, this->child, 0, 0);
        waitpid(this->child, &this->wait_status, 0);
        if (!this->is_stopped()) return;

        // Change the breakpoint to 0xcc
        ptrace(PTRACE_POKETEXT, this->child, *(registers["rip"]),
               (mem & 0xffffffffffffff00) | 0xcc);
    }

    return;
}

// Function for continuing the program execution
void Debugger::continue_the_execution() {
    if (!this->is_running()) return;

    this->run_breakpoint();
    if (!this->is_stopped()) return;

    ptrace(PTRACE_CONT, this->child, 0, 0);
    waitpid(this->child, &this->wait_status, 0);

    this->check_breakpoint();
}

// Function for disassembling 10 instructions at the user-typed address
void Debugger::disassemble_instructions() {
    if (!this->is_stopped()) return;

    if (!this->is_running()) return;

    if (this->command.size() < 2) {
        std::cout << "** No address is given." << std::endl;
        return;
    }

    ull address{this->to_ull(this->command[1])};

    // Extract ten instructions from the given address
    long instructions[10];
    for (int inst_id{0}; inst_id < 10; inst_id++) {
        long mem =
            ptrace(PTRACE_PEEKTEXT, this->child, address + inst_id * 8, 0);

        instructions[inst_id] = mem;
        unsigned char* bytes = (unsigned char*)&(instructions[inst_id]);
        for (int byte_id{0}; byte_id < 8; byte_id++) {
            // If the byte is 0xcc, we need to change it back to its original
            // instruction
            if (bytes[byte_id] == 0xcc) {
                auto it{this->address_to_breakpoint.find(address + inst_id * 8 +
                                                         byte_id)};
                if (it == this->address_to_breakpoint.end()) continue;
                bytes[byte_id] = (it->second).original_instruction;
            }
        }
    }

    // Use capstone to translate the bytes into human-readable instructions
    csh handle;
    if (cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK) {
        perror("** Failed opening capstone");
        return;
    }
    cs_insn* insn;
    size_t count = cs_disasm(handle, (uint8_t*)instructions,
                             sizeof(instructions), address, 10, &insn);
    if (count > 0) {
        for (size_t idx{0}; idx < count; idx++) {
            if (insn[idx].address > this->end_address) {
                std::cout
                    << "** The address is out of the range of the text segment."
                    << std::endl;
                break;
            }
            std::cout << std::hex << std::setw(12) << std::right
                      << insn[idx].address << ":";
            std::stringstream ss;
            for (int byte_idx{0}; byte_idx < insn[idx].size; byte_idx++) {
                ss << " " << std::setw(2) << std::right << std::setfill('0')
                   << std::hex << (unsigned int)insn[idx].bytes[byte_idx];
            }
            std::cout << std::setw(24) << std::left << ss.str() << std::setw(5)
                      << std::left << insn[idx].mnemonic << " "
                      << insn[idx].op_str << std::endl;

            // Undo io manipulation
            std::cout.copyfmt(std::ios(NULL));
        }
        cs_free(insn, count);
    }
    cs_close(&handle);
}

// Function for dumping 80 bytes of the memory content from the program
void Debugger::dump_the_memory() {
    if (!this->is_stopped()) return;

    if (!this->is_running()) return;

    if (this->command.size() < 2) {
        std::cout << "** No address is given." << std::endl;
        return;
    }

    ull address{this->to_ull(this->command[1])};

    for (int line{0}; line < 80 / 16; line++) {
        std::string memory{};
        for (int word{0}; word < 2; word++) {
            long mem = ptrace(PTRACE_PEEKTEXT, this->child, address, 0);
            memory += std::string((char*)&mem, 8);
            address += 8;
        }

        std::cout << std::setw(12) << std::hex << address - 16 << ":";
        for (const unsigned char& ch : memory) {
            std::cout << " " << std::setfill('0') << std::setw(2) << +ch;
        }
        std::cout << "  |";
        for (const auto& ch : memory) {
            std::cout << (std::isprint(ch) ? ch : '.');
        }
        std::cout << "|" << std::endl;

        // Undo io manipulation
        std::cout.copyfmt(std::ios(NULL));
    }
}

// Function for cleaning up the debugger and exiting
void Debugger::quit_from_the_debugger() { this->stop = true; }

// Function for getting the value of a register from the program
void Debugger::get_a_register() {
    if (!this->is_stopped()) return;

    if (!this->is_running()) return;

    if (this->command.size() < 2) {
        std::cout << "** No register is given." << std::endl;
        return;
    }

    std::unordered_map<std::string, ull*> registers{this->fetch_registers()};
    auto it = registers.find(this->command[1]);
    if (it != registers.end())
        std::cout << this->command[1] << " = " << std::dec << *(it->second)
                  << std::hex << " (0x" << *(it->second) << ")" << std::endl;
    else
        std::cout << "** Unknown register: \"" << this->command[1] << "\""
                  << std::endl;

    // Undo io manipulation
    std::cout.copyfmt(std::ios(NULL));
}

// Function for setting the value of a register in the program
void Debugger::set_a_register() {
    if (!this->is_stopped()) return;

    if (!this->is_running()) return;

    if (this->command.size() < 3) {
        std::cout << "** No register or value is given." << std::endl;
        return;
    }

    std::unordered_map<std::string, ull*> registers{this->fetch_registers()};
    *(registers[this->command[1]]) = this->to_ull(this->command[2]);
    ptrace(PTRACE_SETREGS, this->child, 0, &this->regs);
}

// Function for getting values of all registers from the program
void Debugger::get_all_registers() {
    if (!this->is_stopped()) return;

    if (!this->is_running()) return;

    std::unordered_map<std::string, ull*> registers{this->fetch_registers()};
    std::cout << std::hex << "RAX   " << std::setw(16) << std::left
              << *(registers["rax"]) << " RBX   " << std::setw(16) << std::left
              << *(registers["rbx"]) << " RCX   " << std::setw(16) << std::left
              << *(registers["rcx"]) << " RDX   " << std::setw(16) << std::left
              << *(registers["rdx"]) << "\n"
              << "R8    " << std::setw(16) << std::left << *(registers["r8"])
              << " R9    " << std::setw(16) << std::left << *(registers["r9"])
              << " R10   " << std::setw(16) << std::left << *(registers["r10"])
              << " R11   " << std::setw(16) << std::left << *(registers["r11"])
              << "\n"
              << "R12   " << std::setw(16) << std::left << *(registers["r12"])
              << " R13   " << std::setw(16) << std::left << *(registers["r13"])
              << " R14   " << std::setw(16) << std::left << *(registers["r14"])
              << " R15   " << std::setw(16) << std::left << *(registers["r15"])
              << "\n"
              << "RDI   " << std::setw(16) << std::left << *(registers["rdi"])
              << " RSI   " << std::setw(16) << std::left << *(registers["rsi"])
              << " RBP   " << std::setw(16) << std::left << *(registers["rbp"])
              << " RSP   " << std::setw(16) << std::left << *(registers["rsp"])
              << "\n"
              << "RIP   " << std::setw(16) << std::left << *(registers["rip"])
              << " FLAGS " << std::setw(16) << std::right << std::setfill('0')
              << *(registers["eflags"]) << std::endl;

    // Undo io manipulation
    std::cout.copyfmt(std::ios(NULL));
}

// Function for fetching all registers and providing them to
// "get_a_register" and "get_all_registers"
std::unordered_map<std::string, ull*> Debugger::fetch_registers() {
    std::unordered_map<std::string, ull*> registers{};

    if (ptrace(PTRACE_GETREGS, this->child, 0, &this->regs) == 0) {
        registers["rax"] = &this->regs.rax;
        registers["rbx"] = &this->regs.rbx;
        registers["rcx"] = &this->regs.rcx;
        registers["rdx"] = &this->regs.rdx;

        registers["rsp"] = &this->regs.rsp;
        registers["rbp"] = &this->regs.rbp;
        registers["rsi"] = &this->regs.rsi;
        registers["rdi"] = &this->regs.rdi;
        registers["rip"] = &this->regs.rip;

        registers["cs"] = &this->regs.cs;
        registers["ds"] = &this->regs.ds;
        registers["es"] = &this->regs.es;
        registers["fs"] = &this->regs.fs;
        registers["gs"] = &this->regs.gs;
        registers["ss"] = &this->regs.ss;

        registers["r8"] = &this->regs.r8;
        registers["r9"] = &this->regs.r9;
        registers["r10"] = &this->regs.r10;
        registers["r11"] = &this->regs.r11;
        registers["r12"] = &this->regs.r12;
        registers["r13"] = &this->regs.r13;
        registers["r14"] = &this->regs.r14;
        registers["r15"] = &this->regs.r15;

        registers["orig_rax"] = &this->regs.orig_rax;
        registers["eflags"] = &this->regs.eflags;
        registers["fs_base"] = &this->regs.fs_base;
        registers["gs_base"] = &this->regs.gs_base;
    }

    return registers;
}

// Function for printing the help message
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

// Function for loading the user-typed program and executing it
void Debugger::load_a_program() {
    if (this->state != State::init) {
        // The program is already loaded
        std::cout << "** Program \"" << this->program_name
                  << "\" has been loaded. Entry point 0x" << std::hex
                  << this->entry_address << std::endl;

        // Undo io manipulation
        std::cout.copyfmt(std::ios(NULL));
        return;
    }

    if (this->command.size() < 2) {
        std::cout << "** No program is given." << std::endl;
        return;
    }

    // Fork the tracee
    if ((this->child = fork()) < 0) {
        perror("** Fork");
        this->quit_from_the_debugger();
        return;
    }

    if (child == 0) {
        // Tracee
        if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
            perror("** Ptrace (traceme)");
            this->quit_from_the_debugger();
            return;
        }

        // Execute the program
        execlp(this->command[1].c_str(), this->command[1].c_str());

        // Shouldn't reach here
        perror("** Exec");
        this->quit_from_the_debugger();
    }

    // Tracer
    if (waitpid(this->child, &this->wait_status, 0) < 0) {
        perror("** Wait (after fork)");
        this->quit_from_the_debugger();
        return;
    }

    if (!this->is_stopped()) return;

    ptrace(PTRACE_SETOPTIONS, this->child, 0, PTRACE_O_EXITKILL);
    this->program_name = this->command[1];

    // Read ELF and section headers to get the addresses of entry and end of
    // .text
    FILE* file = fopen(this->program_name.c_str(), "r");
    Elf64_Ehdr elf_header{};
    Elf64_Shdr* sh_table{};
    size_t dummy{};
    dummy++;  // prevent the compiler from showing the unused warning

    fseek(file, 0, SEEK_SET);
    dummy = fread(&elf_header, sizeof(Elf64_Ehdr), 1, file);
    sh_table = (Elf64_Shdr*)malloc(elf_header.e_shentsize * elf_header.e_shnum);

    this->entry_address = elf_header.e_entry;

    fseek(file, elf_header.e_shoff, SEEK_SET);
    dummy =
        fread(sh_table, elf_header.e_shentsize * elf_header.e_shnum, 1, file);

    char* sh_str = (char*)malloc(sh_table[elf_header.e_shstrndx].sh_size);

    if (sh_str != NULL) {
        fseek(file, sh_table[elf_header.e_shstrndx].sh_offset, SEEK_SET);
        dummy = fread(sh_str, sh_table[elf_header.e_shstrndx].sh_size, 1, file);
    }

    for (int idx{0}; idx < elf_header.e_shnum; idx++) {
        if (!strcmp(".text", (sh_str + sh_table[idx].sh_name))) {
            this->end_address =
                (ull)sh_table[idx].sh_addr + sh_table[idx].sh_size - 1;
            break;
        }
    }

    free(sh_table);
    free(sh_str);
    fclose(file);

    std::cout << "** Program \"" << this->program_name
              << "\" loaded. Entry point 0x" << std::hex << this->entry_address
              << std::endl;

    // Undo io manipulation
    std::cout.copyfmt(std::ios(NULL));

    this->state = State::loaded;
}

// Function for running the program
void Debugger::run_the_program() {
    if (this->state == State::running) {
        std::cout << "** Program \"" << this->program_name
                  << "\" is already running" << std::endl;
        this->continue_the_execution();
        return;
    }

    if (this->state == State::loaded) {
        this->start_the_program();
        return;
    }

    // TODO: need to handle re-run after tracee exited

    std::cout << "** \"run\" is for state loaded and running only."
              << std::endl;
}

// Function for showing the memory layout of the running program
void Debugger::show_memory_layout() {
    if (!this->is_running()) return;

    // Get memory layout from the tracee's maps
    std::ifstream ifs{"/proc/" + std::to_string(this->child) + "/maps"};
    while (ifs.good()) {
        std::string line{};
        std::getline(ifs, line);
        std::vector<std::string> tokens{tokenize(line)};

        if (tokens.size() >= 6) {
            // Extract the start address and end address
            std::string start{}, end{};
            std::stringstream ss{tokens[0]};
            std::getline(ss, start, '-');
            std::getline(ss, end);

            ull field{this->to_ull(tokens[2])};
            std::cout << std::setw(16) << std::setfill('0') << start << "-"
                      << std::setw(16) << std::setfill('0') << end << " "
                      << tokens[1] << " " << field << "\t" << tokens[5]
                      << std::endl;

            // Undo io manipulation
            std::cout.copyfmt(std::ios(NULL));
        }
    }
    ifs.close();
}

// Function for running a single instruction and stepping into the function call
void Debugger::run_a_single_instruction() {
    if (!this->is_running()) return;

    this->run_breakpoint();
    if (!this->is_stopped()) return;

    ptrace(PTRACE_SINGLESTEP, this->child, 0, 0);
    waitpid(this->child, &this->wait_status, 0);

    this->check_breakpoint();
}

// Function for starting the program and stopping at the first instruction
void Debugger::start_the_program() {
    if (this->state == State::running) {
        std::cout << "** Program \"" << this->program_name
                  << "\" is already running" << std::endl;
        return;
    }

    if (this->state == State::init) {
        std::cout << "** The program is not loaded." << std::endl;
        return;
    }

    std::cout << "** PID " << this->child << std::endl;
    this->state = State::running;
}