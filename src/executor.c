#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "executor.h"
// #include "parser.h"

// 함수 주석 이렇게 적으면 마우스 오버했을 때 도움말 뜸 나중에 써보슈 ㄱㄱ
/**
 * @brief 주어진 Command를 실행한다.
 * 
 * @param cmd 실행할 Command와 인자
 * @return int 성공(0), 실패(-1)
 */
int execute_command(Command cmd) {
    pid_t pid = fork();
    
    // 만약 fork로 자식프로세스 생성이 실패하면 -1 리턴
    if (pid < 0) {
        perror("fork failed");
        return -1;
    }

    if (pid == 0) {
        // 자식 프로세스
        if (execvp(cmd.name, cmd.args) == -1) {
            fprintf(stderr, "에러: '%s'은(는) 명령, 실행 가능한 프로그램, 또는 빌드 파일이 아닙니다..\n", cmd.name);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        // 부모 프로세스의 입장에서

        int status;
        // 자식 프로세스가 종료될 때까지 대기
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid failed");
            return -1;
        }

        // 자식 프로세스가 종료되었는지 확인
        if (WIFEXITED(status)) {
            // 만약 정상적으로 종료되었다면
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0) {
                fprintf(stderr, "'%s'를 코드 %d으로 종료되었습니다.\n", cmd.name, exit_status);
                return -1;
            }
        } else if (WIFSIGNALED(status)) {
            // 만약 자식 프로세스가 인터럽트 같은 외부 신호로 종료되었다면
            int term_sig = WTERMSIG(status);
            fprintf(stderr, "'%s'는 신호 %d에 의해 종료되었습니다.\n", cmd.name, term_sig);
            return -1;
        }
    }

    return 0;
}
