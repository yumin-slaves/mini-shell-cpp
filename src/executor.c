#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "executor.h"
#include "parser.h"

int handle_internal_command(Command cmd) {
    if (strcmp(cmd.name, "exit") == 0) {
        printf("Bye!\n");
        exit(0);
    }
    if (strcmp(cmd.name, "cd") == 0) {
        if (cmd.args[1] == NULL) {
            fprintf(stderr, "cd: 경로를 입력하세요.\n");
        } else if (chdir(cmd.args[1]) != 0) {
            perror("cd 실패");
        }
        return 1; // 내부 명령 처리 완료
    }
    if (strcmp(cmd.name, "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("pwd 실패");
        }
        return 1; // 내부 명령 처리 완료
    }
    if (strcmp(cmd.name, "export") == 0) {
        if (cmd.args[1] == NULL) {
            fprintf(stderr, "export: 변수=값 형식으로 입력하세요.\n");
        } else {
            char* equal_sign = strchr(cmd.args[1], '=');
            if (!equal_sign) {
                fprintf(stderr, "export: 변수=값 형식이 아닙니다.\n");
            } else {
                *equal_sign = '\0';
                char* key = cmd.args[1];
                char* value = equal_sign + 1;
                if (setenv(key, value, 1) != 0) {
                    perror("환경변수 설정 실패");
                }
            }
        }
        return 1; // 내부 명령 처리 완료
    }
    return 0; // 외부 명령 실행 필요
}


int execute_commands(Command* cmds, int num_cmds) {
 
    int pipe_fd[2] = {-1,-1};
    int prev_read_fd = -1;

    for(int i = 0; i < num_cmds; i++){
        // 마지막 명령이 아닐 때만 pipe 생성
        if (i < num_cmds - 1){
            if(pipe(pipe_fd) == -1){
                perror("pipe()");
                return -1;
            }
        }

        // 내부 명령어 처리
        if (handle_internal_command(cmds[i])){
            if (prev_read_fd != -1) close(prev_read_fd);
            if (pipe_fd[0] != -1) close(pipe_fd[0]);
            if (pipe_fd[1] != -1) close(pipe_fd[1]);
            continue;
        }

        pid_t pid = fork();

        if (pid < 0){
            perror("fork");
            return -1;
        }

        if (pid == 0){

            if (prev_read_fd != -1){
                dup2(prev_read_fd, STDIN_FILENO);
                close(prev_read_fd);
            }
            if (i < num_cmds - 1){
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }

            if (execvp(cmds[i].name, cmds[i].args) == -1){
                perror("execvp");
                exit(EXIT_FAILURE);
            }

        }
        else{

            waitpid(pid, NULL, 0);

            if (prev_read_fd != -1){
                close(prev_read_fd);
            }

            if (i < num_cmds - 1){
                close(pipe_fd[1]);
                prev_read_fd = pipe_fd[0];
            }

        }

    }

    if (prev_read_fd != -1) {
        close(prev_read_fd);
    }

    for(int i = 0; i < num_cmds ;i++){
        wait(NULL);
    }

    return 0;
}
