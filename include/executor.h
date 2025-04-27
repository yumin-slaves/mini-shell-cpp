typedef struct {
    char *name;
    char **args;
} Command;

void execute_command(Command cmd);