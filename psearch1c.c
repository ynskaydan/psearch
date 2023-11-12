//
// Created by Yunus Kaydan on 6.11.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void fileSearch(const char *keyword, const char *inputFileName, int *pipeline) {
    FILE *inputFileTool = fopen(inputFileName, "r"); // its a tool that reads from file and converts to str
    if (inputFileTool == NULL) { // if input file or temporary file is null give an error
        perror("no input file or error in temp output file");
        exit(1);
    }

    char str[1024];  // line array
    char index = 0;
    while (fgets(str, sizeof(str), inputFileTool) != NULL) { // reads file
        if (strstr(str, keyword) != NULL) {
            char matchedLine[1024];
            snprintf(matchedLine, sizeof(matchedLine), "%s, %d: %s", inputFileName, index, str);
            write(pipeline[1], matchedLine, strlen(matchedLine)); // write matchedLine to pipeline
        }
        index++;
    }
    fclose(inputFileTool);
    close(pipeline[1]);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("program should be: psearch name 3 input1.txt input2.txt input3.txt output.txt");
        exit(1);
    }
    // Identifying parameters of psearch
    char *keyword = argv[1];  // an alphanumeric keyword
    int n = atoi(argv[2]); // number of input files (n)

    if (n <= 1) { // constraint
        printf("n should be greater than or equal to 1");
        exit(1);
    }

    int pipeline[2];  // Pipe file descriptors
    if (pipe(pipeline) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) { // for each file creating child process
        pid_t pid = fork();

        if (pid == -1) { // fork error
            perror("Fork Error");
            exit(1);
        } else if (pid == 0) { // child process
            char *inputFileName = argv[i + 3];
            close(pipeline[0]);  // read end of the pipe
            fileSearch(keyword, inputFileName, pipeline);
            exit(0);
        }
    }
    close(pipeline[1]);

    for (int i = 0; i < n; i++) { // parent process waits all children
        wait(NULL);
    }

    char *outputFile = argv[3 + n];
    FILE *outputFileTool = fopen(outputFile, "w");

    char buffer[1024];
    while (read(pipeline[0], buffer, sizeof(buffer)) > 0) {
        fprintf(outputFileTool, "%s", buffer);
    }

    fclose(outputFileTool);
    close(pipeline[0]);  // Close the read end of the pipe
    return 0;
}






