#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "redirect.h"

/**
 *  @brief redirection 기능 구현
 *  @enum RedirectType : redirection 상태 표현
 *  @param file_name
 *  @return 성공 0, 실패 -1
 */

 int handle_input_redirect(char *input_file){

    int input_fd = open(input_file, O_RDONLY);
    if(input_fd == -1){
        perror("Input file open failed");
        return -1;
    }
    dup2(input_fd, STDIN_FILENO);
    close(input_fd);
    return 0;

 }

 int handle_output_redirect(char *output_file){

    int output_fd = open(output_file,  O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(output_fd == -1){
        perror("Output file open failed");
        return -1;
    }
    dup2(output_fd,STDOUT_FILENO);
    close(output_fd);
    return 0;
 }

 int handle_append_redirect(char *output_file){
    int outpud_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if(outpud_fd == -1){
        perror("Output file open failed");
        return -1;
    }
    dup2(outpud_fd, STDOUT_FILENO);
    close(outpud_fd);
    return 0;
 }