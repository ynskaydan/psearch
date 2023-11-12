//
// Created by Yunus Kaydan on 6.11.2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void fileSearch(const char *keyword, const char *inputFileName, FILE *tempOutputFileTool) {
    FILE *inputFileTool = fopen(inputFileName, "r"); // its a tool that reads from file and converts to str
    if (inputFileTool == NULL || tempOutputFileTool == NULL) { // if file is null give an error
        perror("no input file or error in temp output file");
        exit(1);
    }
    char str[1024];  // line array
    char index = 0;
    while (fgets(str, sizeof(str), inputFileTool) != NULL) { // reads file
        if (strstr(str, keyword) != NULL) {
            fprintf(tempOutputFileTool, "%s, %d: %s", inputFileName, index, str);
        }
        index++;
    }
    fclose(inputFileTool);
    fclose(tempOutputFileTool);
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

    FILE *tempOutputFiles[n]; // temporary output fiels which will be use in  section

    for (int i = 0; i < n; i++) { // for each file creating child process
        pid_t pid = fork();

        if (pid == -1) { // fork error
            perror("Fork Error");
            exit(1);
        } else if (pid == 0) { // child process
            char *inputFileName = argv[i + 3]; // input1.txt input2.txt input3.txt
            char tempOutputFileName[100];  // tempoutput file name its using for complex name
            sprintf(tempOutputFileName, "%s%d%s", "tempOutput-", i, ".txt");
            tempOutputFiles[i] = fopen(tempOutputFileName, "w");
            fileSearch(keyword, inputFileName,
                       tempOutputFiles[i]); // keyword will search and matched line will write to temp output file
            exit(0);
        }
    }

    for (int i = 0; i < n; i++) { // parent process waits all children
        wait(NULL);
    }

    char *outputFile = argv[3 + n];
    FILE *outputFileTool = fopen(outputFile, "w");

    for (int i = 0; i < n; i++) {
        char tempOutputFileName[100];
        sprintf(tempOutputFileName, "%s%d%s", "tempOutput-", i, ".txt");
        tempOutputFiles[i] = fopen(tempOutputFileName, "r");
        if (tempOutputFiles[i] == NULL) {
            perror("error opening temporary output file");
            exit(1);
        }

        char str[1024];
        while (fgets(str, sizeof(str), tempOutputFiles[i]) != NULL) {
            fprintf(outputFileTool, "%s", str); // merges all temporary output files to output file
        }

        fclose(tempOutputFiles[i]);
        remove(tempOutputFileName);
    }

    fclose(outputFileTool);
    return 0;
}


