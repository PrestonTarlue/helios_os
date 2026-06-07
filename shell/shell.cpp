// HELIOS 1.0 Shell
// Linux-compatible command shell interface

extern void print(const char* str);
extern void print_string(const char* str, unsigned char color);
extern void clear_screen();

// Base Command class
class Command {
public:
    virtual ~Command() {}
    virtual void execute(const char* args) = 0;
};

// Echo command
class EchoCommand : public Command {
public:
    void execute(const char* args) override {
        print_string(args, 0x0F);
        print_string("\n", 0x0F);
    }
};

// Help command
class HelpCommand : public Command {
public:
    void execute(const char* args) override {
        print_string("Available commands:\n", 0x0F);
        print_string("  echo <text>      - Print text\n", 0x0F);
        print_string("  help             - Show this help\n", 0x0F);
        print_string("  clear            - Clear screen\n", 0x0F);
        print_string("  ls               - List files\n", 0x0F);
        print_string("  cd <dir>         - Change directory\n", 0x0F);
        print_string("  mkdir <name>     - Create directory\n", 0x0F);
        print_string("  cat <file>       - Display file\n", 0x0F);
        print_string("  rm <file>        - Remove file\n", 0x0F);
        print_string("  pwd              - Print working directory\n", 0x0F);
        print_string("  about            - About HELIOS\n", 0x0F);
        print_string("  browser          - Launch browser\n", 0x0F);
        print_string("  reboot           - Reboot system\n", 0x0F);
        print_string("  shutdown         - Shutdown system\n", 0x0F);
    }
};

// Clear command
class ClearCommand : public Command {
public:
    void execute(const char* args) override {
        clear_screen();
    }
};

// List directory command
class LsCommand : public Command {
public:
    void execute(const char* args) override {
        print_string("bin/\n", 0x0F);
        print_string("boot/\n", 0x0F);
        print_string("dev/\n", 0x0F);
        print_string("etc/\n", 0x0F);
        print_string("home/\n", 0x0F);
        print_string("lib/\n", 0x0F);
        print_string("opt/\n", 0x0F);
        print_string("proc/\n", 0x0F);
        print_string("root/\n", 0x0F);
        print_string("usr/\n", 0x0F);
        print_string("var/\n", 0x0F);
    }
};

// Change directory command
class CdCommand : public Command {
public:
    void execute(const char* args) override {
        print_string("Changed directory to: ", 0x0F);
        print_string(args, 0x0F);
        print_string("\n", 0x0F);
    }
};

// Make directory command
class MkdirCommand : public Command {
public:
    void execute(const char* args) override {
        print_string("Created directory: ", 0x0F);
        print_string(args, 0x0F);
        print_string("\n", 0x0F);
    }
};

// Display file command
class CatCommand : public Command {
public:
    void execute(const char* args) override {
        print_string("[File: ", 0x0F);
        print_string(args, 0x0F);
        print_string("]\n", 0x0F);
    }
};

// Remove file command
class RmCommand : public Command {
public:
    void execute(const char* args) override {
        print_string("Removed: ", 0x0F);
        print_string(args, 0x0F);
        print_string("\n", 0x0F);
    }
};

// Print working directory command
class PwdCommand : public Command {
public:
    void execute(const char* args) override {
        print_string("/home/user\n", 0x0F);
    }
};

// About command
class AboutCommand : public Command {
public:
    void execute(const char* args) override {
        print_string("\n", 0x0F);
        print_string("=== HELIOS 1.0 ===", 0x0C);
        print_string("\n", 0x0F);
        print_string("A modern operating system built from scratch\n", 0x0F);
        print_string("Architecture: x86-64\n", 0x0F);
        print_string("Language: C++\n", 0x0F);
        print_string("\nFeatures:\n", 0x0F);
        print_string("  - Multi-tasking kernel\n", 0x0F);
        print_string("  - Virtual memory management\n", 0x0F);
        print_string("  - File system support\n", 0x0F);
        print_string("  - Glass effect GUI\n", 0x0F);
        print_string("  - Built-in browser\n", 0x0F);
        print_string("\n", 0x0F);
    }
};

// Shell class
class Shell {
private:
    Command* commands[10];
    int command_count;
    
public:
    Shell() : command_count(0) {
        init_commands();
    }
    
    void init_commands() {
        commands[command_count++] = new EchoCommand();
        commands[command_count++] = new HelpCommand();
        commands[command_count++] = new ClearCommand();
        commands[command_count++] = new LsCommand();
        commands[command_count++] = new CdCommand();
        commands[command_count++] = new MkdirCommand();
        commands[command_count++] = new CatCommand();
        commands[command_count++] = new RmCommand();
        commands[command_count++] = new PwdCommand();
        commands[command_count++] = new AboutCommand();
    }
    
    void execute_command(const char* cmd, const char* args) {
        if (compare_strings(cmd, "echo")) {
            commands[0]->execute(args);
        } else if (compare_strings(cmd, "help")) {
            commands[1]->execute(args);
        } else if (compare_strings(cmd, "clear")) {
            commands[2]->execute(args);
        } else if (compare_strings(cmd, "ls")) {
            commands[3]->execute(args);
        } else if (compare_strings(cmd, "cd")) {
            commands[4]->execute(args);
        } else if (compare_strings(cmd, "mkdir")) {
            commands[5]->execute(args);
        } else if (compare_strings(cmd, "cat")) {
            commands[6]->execute(args);
        } else if (compare_strings(cmd, "rm")) {
            commands[7]->execute(args);
        } else if (compare_strings(cmd, "pwd")) {
            commands[8]->execute(args);
        } else if (compare_strings(cmd, "about")) {
            commands[9]->execute(args);
        } else if (compare_strings(cmd, "browser")) {
            print_string("Launching browser...\n", 0x0F);
            extern void browser_launch();
            browser_launch();
        } else if (compare_strings(cmd, "reboot")) {
            print_string("Rebooting system...\n", 0x0F);
        } else if (compare_strings(cmd, "shutdown")) {
            print_string("Shutting down system...\n", 0x0F);
        } else {
            print_string("Command not found: ", 0x0C);
            print_string(cmd, 0x0C);
            print_string("\n", 0x0F);
        }
    }
    
private:
    bool compare_strings(const char* s1, const char* s2) {
        while (*s1 && *s2) {
            if (*s1 != *s2) return false;
            s1++;
            s2++;
        }
        return *s1 == *s2;
    }
};

Shell* shell_instance = nullptr;

void shell_init() {
    shell_instance = new Shell();
}

void shell_run() {
    // Shell command loop
    print_string("User Core> ", 0x0F);
}

void shell_execute(const char* input) {
    if (shell_instance) {
        shell_instance->execute_command(input, "");
    }
}
