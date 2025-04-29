typedef struct Command{
    char* name;
    char **args;
    int argc;
} Command;

int execute_command(Command cmd);