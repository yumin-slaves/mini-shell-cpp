#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "executor.h"
#include "parser.h"

int execute_commands(Command* cmds, int num_cmds) {
    int i;
    int prev_pipe_fd[2] = {-1, -1};

    for (i = 0; i < num_cmds; i++) {
        int pipe_fd[2];

        if (i < num_cmds - 1) {
            if (pipe(pipe_fd) == -1) {
                perror("pipe");
                return -1;
            }
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return -1;
        }

        if (pid == 0) {  // 자식 프로세스
            // stdin 연결: 이전 파이프의 read end
            if (i > 0) {
                dup2(prev_pipe_fd[0], STDIN_FILENO);
                close(prev_pipe_fd[0]);
                close(prev_pipe_fd[1]);
            }

            // stdout 연결: 다음 파이프의 write end
            if (i < num_cmds - 1) {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }

            // 내부 명령어 처리
            if (strcmp(cmds[i].name, "exit") == 0) {
                printf("Bye!\n");
                exit(0);
            }
            if (strcmp(cmds[i].name, "cd") == 0) {
                if (cmds[i].args[1] == NULL) {
                    fprintf(stderr, "cd: 경로를 입력하세요.\n");
                } else if (chdir(cmds[i].args[1]) != 0) {
                    perror("cd 실패");
                }
                exit(0);
            }
            if (strcmp(cmds[i].name, "pwd") == 0) {
                char cwd[1024];
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    printf("%s\n", cwd);
                } else {
                    perror("pwd 실패");
                }
                exit(0);
            }
            if (strcmp(cmds[i].name, "export") == 0) {
                if (cmds[i].args[1] == NULL) {
                    fprintf(stderr, "export: 변수=값 형식으로 입력하세요.\n");
                } else {
                    char* equal_sign = strchr(cmds[i].args[1], '=');
                    if (!equal_sign) {
                        fprintf(stderr, "export: 변수=값 형식이 아닙니다.\n");
                    } else {
                        *equal_sign = '\0';
                        char* key = cmds[i].args[1];
                        char* value = equal_sign + 1;
                        if (setenv(key, value, 1) != 0) {
                            perror("환경변수 설정 실패");
                        }
                    }
                }
                exit(0);
            }

            // 외부 명령 실행
            if (execvp(cmds[i].name, cmds[i].args) == -1) {
                fprintf(stderr, "에러: '%s' 실행 실패\n", cmds[i].name);
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        } else {  // 부모 프로세스
            // 이전 파이프 닫기
            if (i > 0) {
                close(prev_pipe_fd[0]);
                close(prev_pipe_fd[1]);
            }

            // 현재 파이프를 이전 파이프로 보관
            if (i < num_cmds - 1) {
                prev_pipe_fd[0] = pipe_fd[0];
                prev_pipe_fd[1] = pipe_fd[1];
                close(pipe_fd[1]);  // 부모는 write end 닫음
            }
        }
    }

    // 모든 자식 종료 대기
    for (i = 0; i < num_cmds; i++) {
        int status;
        wait(&status);
    }

    return 0;
}
