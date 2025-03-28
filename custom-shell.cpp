#include "custom-shell.h"
#include "ansi-color-codes/ansi-color-codes.h"
#include <algorithm>

CustomShell* CustomShell::customShellInstance = nullptr;

CustomShell::CustomShell(std::string _shell_prompt)
{
    SHELL_PROMPT = _shell_prompt;
    customShellInstance = this;
    rl_attempted_completion_function = command_completion;
    register_variable("verbose_wtf", &verbose_wtf, "int");
    register_command("quit", quit);
    register_command("exit", quit);

}

int CustomShell::quit(const std::string &arg)
{
    customShellInstance->running = false;
    return 0;
}

void CustomShell::run()
{
    running = true;

    strcpy(user_in_prev, "");
    while(running)
    {
        if((user_in = readline(SHELL_PROMPT.c_str())) != nullptr)
        {
            if(strcmp(user_in, user_in_prev) != 0) add_history(user_in);
            strcpy(user_in_prev, user_in);

            execute_command(user_in);
            free(user_in);
        }
    }
}

void CustomShell::execute_command(std::string user_in)
{
    if(user_in.empty())
    {
        return;
    }
    trim_str(&user_in);

    size_t delim_index = user_in.find(CMD_DELIMITER);
    std::string cmd = user_in.substr(0, delim_index); 
    std::string arg = (delim_index != std::string::npos) ? user_in.substr(delim_index + 1) : ""; 
    trim_str(&cmd);
    trim_str(&arg);

    if(verbose_wtf)
    {
        printf("COMMAND:\t%s\n", cmd.c_str());
        printf("ARGUMENT:\t%s\n", arg.c_str());
    }

    auto var_it = var_map.find(cmd);
    auto cmd_it = cmd_map.find(cmd);
    if(var_it != var_map.end())
    {
        CustomShell::Variable this_var = var_it->second;
        if(arg.empty() || (arg.find("=") == 0 && arg.size() == 1))
        {
            this_var.print();
        }
        else if(arg.find("=") != std::string::npos)
        {
            std::string assignment = arg.substr(arg.find("=") + 1);
            trim_str(&assignment);
            this_var.set(assignment);
        }
    }
    else if(cmd_it != cmd_map.end())
    {
        cmd_it->second(arg);
    }
    else
    {
        printf(RED "ERROR: Unrecognized Command or variable '%s'\n" RESET, user_in.c_str());
    }

}

void CustomShell::Variable::print()
{
    if(type == "int")
        printf("%d\n", *(int*)var);
    if(type == "float")
        printf("%f\n", *(float*)var);
    if(type == "double")
        printf("%lf\n", *(double*)var);
    if(type == "string")
        printf("%s\n", (*(std::string*)var).c_str());
}

void CustomShell::Variable::set(const std::string &assignment)
{
    try
    {
        if(type == "int")
            *static_cast<int*>(var) = std::stoi(assignment);
        if(type == "float")
            *static_cast<float*>(var) = std::stof(assignment);
        if(type == "double")
            *static_cast<double*>(var) = std::stod(assignment);
        if(type == "string")
            *static_cast<std::string*>(var) = assignment;
        print();
    }
    catch(...)
    {
        printf(RED "ERROR: Invalid setting for variable of type '%s'\n" RESET, type.c_str());

    }
}

void CustomShell::set_prompt(const std::string &prompt)
{
    SHELL_PROMPT = prompt;
}

void CustomShell::register_variable(std::string cli_name, void* variable, std::string type)
{
    var_map[cli_name] = {variable, type};
}

void CustomShell::register_command(std::string cli_name, std::function<int(const std::string &arg)> func)
{
    cmd_map[cli_name] = func;
}

char* CustomShell::command_generator(const char *text, int state) 
{
    static std::vector<std::string> matches;
    static size_t match_index = 0;

    if(state == 0)
    {
        matches.clear();
        match_index = 0;

        std::string textstr(text);
        for(auto key : customShellInstance->cmd_map)
        {
            if(key.first.size() >= textstr.size() && key.first.compare(0, textstr.size(), textstr) == 0) 
            {
                matches.push_back(key.first);
            }
        }
        for(auto key : customShellInstance->var_map)
        {
            if(key.first.size() >= textstr.size() && key.first.compare(0, textstr.size(), textstr) == 0) 
            {
                matches.push_back(key.first);
            }
        }
    }

    if(match_index >= matches.size())
    {
        return nullptr;
    } 
    else 
    {
        return strdup(matches[match_index++].c_str());
    }
}

char** CustomShell::command_completion(const char *text, int start, int end)
{
    CustomShell* instance = customShellInstance;

    return rl_completion_matches(text, instance->command_generator);
}

void CustomShell::trim_str(std::string* str)
{
    str->erase(str->find_last_not_of("\t\n\v\f\r ") + 1); // right trim
    str->erase(0, str->find_first_not_of("\t\n\v\f\r ")); // left trim
}

void CustomShell::trim_cstr(char* &str)
{
    char* end = str + strlen(str) - 1;

    // trim leading whitespace
    while(std::isspace((unsigned char)* str)) str++;

    // trim trailing whitespace
    while(end > str && std::isspace((unsigned char)* end)) end--;
    end[1] = '\0';
}

std::vector<std::string> CustomShell::split_arg(std::string arg)
{
    std::vector<std::string> out;
    std::string CMD_DELIMITER = " ";
    
    std::string token;
    size_t delim_index;
    while((delim_index = arg.find(CMD_DELIMITER)) != std::string::npos)
    {
        token = arg.substr(0, delim_index);
        out.push_back(token);
        arg.erase(0, delim_index + CMD_DELIMITER.length());
    }
    out.push_back(arg);

    return out;
}
