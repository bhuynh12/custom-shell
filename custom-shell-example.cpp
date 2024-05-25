#include <algorithm>
#include <iostream>
#include <readline/history.h>
#include <string>
#include <vector>
#include <string.h>
#include <readline/readline.h>
#include <unordered_map>
#include <functional>
#include <chrono>
#include "custom-shell.h"

#define RESET       "\033[0m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"

auto curr_time = std::chrono::steady_clock::now();
auto prev_time = curr_time;
uint64_t dt_ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_time - prev_time).count();

std::string SHELL_PROMPT = "my-shell> ";
std::string CMD_DELIMITER = " ";
int verbose_wtf = 0;
int random_int = 5;
float random_float = 10.4;
double random_double = 3.1415926;
std::string random_string = "hello";

uint64_t unix_time_ms();
int foo(const std::string &arg = "");
int bar(const std::string &arg = "");

int main(int argc, char** argv)
{
    CustomShell myshell(SHELL_PROMPT);
    myshell.register_variable("random_int", &random_int, "int");
    myshell.register_variable("random_float", &random_float, "float");
    myshell.register_variable("random_double", &random_double, "double");
    myshell.register_variable("random_string", &random_string, "string");

    myshell.register_command("foo", foo);
    myshell.register_command("bar", bar);
    myshell.run();
}


uint64_t unix_time_ms()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int foo(const std::string &arg)
{
    std::vector<std::string> argv = CustomShell::split_arg(arg);
    printf("arg1 = %s\n", argv[0].c_str());
    printf("arg2 = %s\n", argv[1].c_str());
    printf("arg3 = %s\n", argv[2].c_str());
    return 0;
}
int bar(const std::string &arg)
{
    std::vector<std::string> argv = CustomShell::split_arg(arg);
    printf("time: %lu\n", unix_time_ms());
    return 0;
}
