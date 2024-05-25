# CUSTOM-SHELL
---
Welcome to the custom-shell repo!

This project is a library for running a custom shell

## Adding Mutable Variables
---
Mutable variables can be added to the shell like this:

```
CustomShell myshell();
myshell.register_variable("random_int", &random_int, "int");
```

Theses variables' values can be changed in real time during the shell execution
by typing something like this in the shell and pressing enter:

```
random_int = 4
```

available data types to be registered are:
* int
* float
* double
* std::string

## Adding Shell Commands
---
Custom commands can be written and run in the shell. These commands are
functions must have an int return type and can only have one parameter of type
const std::string, for example:

```
int foo(const std::string &arg = "");

int foo(const std::string &arg)
{
    std::vector<std::string> argv = CustomShell::split_arg(arg);
    printf("arg1 = %s\n", argv[0].c_str());
    printf("arg2 = %s\n", argv[1].c_str());
    printf("arg3 = %s\n", argv[2].c_str());
    return 0;
}
```

This function would then be registered into the shell like so:

```
myshell.register_command("foo", foo);
```

## Notes
---
* All registered variables and functions can be tab completed on the shell
* To see the current value of a variable, just run the variable name by itself
