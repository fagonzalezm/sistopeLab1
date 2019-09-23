#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
    //Code
    char word[6];
    read(STDIN_FILENO, word, 6);
    strcat(word,"4");
    write(STDOUT_FILENO, word, 6);

    wait(NULL);
    //Send Results
    return 0;
}