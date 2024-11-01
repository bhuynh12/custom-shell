#ifndef CUSTOMSHELLH
#define CUSTOMSHELLH

#include <cstring>
#include <functional>
#include <string>
#include <readline/readline.h>
#include <readline/history.h>
#include <unordered_map>
#include <vector>

class CustomShell
{
    public:
        CustomShell(std::string _shell_prompt = "myshell> ");
        void set_prompt(const std::string &prompt);
        void register_variable(std::string cli_name, void* variable, std::string type);
        void register_command(std::string cli_name, std::function<int(const std::string &arg)> func);
        void run();
        static int quit(const std::string &arg = "");

        static std::vector<std::string> split_arg(std::string arg);

    private:
        static CustomShell* customShellInstance;
        std::string SHELL_PROMPT;
        std::string CMD_DELIMITER = " ";
        struct Variable
        {
            void* var;
            std::string type;

            void print();
            void set(const std::string &assignment);
        };
        std::unordered_map<std::string, Variable> var_map;
        std::unordered_map<std::string, std::function<int(const std::string &arg)>> cmd_map;
        bool running = false;
        char* user_in;
        char user_in_prev[100];

        static char* command_generator(const char *text, int state);
        static char** command_completion(const char *text, int start, int end);

        void trim_str(std::string &str);
        void trim_cstr(char* &str);

        void execute_command(std::string user_in);

        // CLI variables/commands
        int verbose_wtf = 0;

};


#endif
