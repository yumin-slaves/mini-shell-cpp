#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "redirect.h"

/**
 * @brief 표준 입력을 파일로 리디렉션합니다.
 *
 * @param input_file 입력에 사용할 파일 이름
 * @return 성공 시 0, 실패 시 -1
 */
int handle_input_redirect(char* input_file) {
    int input_fd = open(input_file, O_RDONLY);
    if (input_fd == -1) {
        perror("Input file open failed");
        return -1;
    }
    dup2(input_fd, STDIN_FILENO); // 표준 입력을 파일로 대체
    close(input_fd);
    return 0;
}

/**
 * @brief 표준 출력을 지정된 파일로 리디렉션합니다 (기존 내용은 덮어씌움).
 *
 * @param output_file 출력에 사용할 파일 이름
 * @return 성공 시 0, 실패 시 -1
 */
int handle_output_redirect(char* output_file) {
    int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        perror("Output file open failed");
        return -1;
    }
    dup2(output_fd, STDOUT_FILENO); // 표준 출력을 파일로 대체
    close(output_fd);
    return 0;
}

/**
 * @brief 표준 출력을 지정된 파일로 리디렉션합니다 (기존 내용 뒤에 추가).
 *
 * @param output_file 출력에 사용할 파일 이름
 * @return 성공 시 0, 실패 시 -1
 */
int handle_append_redirect(char* output_file) {
    int output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (output_fd == -1) {
        perror("Output file open failed");
        return -1;
    }
    dup2(output_fd, STDOUT_FILENO); // 표준 출력을 파일로 대체
    close(output_fd);
    return 0;
}
