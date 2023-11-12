//
// Created by Yunus Kaydan on 6.11.2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("program should have minimum 4 parameters like keyword, number of input file, input file, output file");
        exit(1);
    }
    // Identifying parameters of psearch
    char *keyword = argv[1];  // an alphanumeric keyword
    int n = atoi(argv[2]); // number of input files (n)

    if (n <= 1) { // constraint
        printf("n should be greater than or equal to 1");
        exit(1);
    }

    char *shared_memory; // Creates a shared memory for storing outputs
    size_t shared_memory_size = n * 1024; // bounded shared memory
    shared_memory = mmap(NULL, shared_memory_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); // map shared memory using mmap

    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    sem_t *semaphore; // usign anonymous semaphore for syncronization of shared memory
    semaphore = sem_open("/sharedMemory", 1, 1);

    char *outputFile = argv[3 + n];
    FILE *outputFileTool = fopen(outputFile, "w");

    for (int i = 0; i < n; i++) { // for each file creating child process
        pid_t pid = fork();

        if (pid == -1) { // fork error
            perror("Fork Error");
            exit(1);
        } else if (pid == 0) { // child process
            char *inputFileName = argv[i + 3];
            FILE *inputFileTool = fopen(inputFileName, "r"); // its a tool that reads from file and converts to str

            char str[1024];  // line array
            char index = 0;
            while (fgets(str, sizeof(str), inputFileTool) != NULL) { // reads file
                if (strstr(str, keyword) != NULL) {
                    sem_wait(semaphore); // syncronization of shared memory
                    snprintf(shared_memory + i * 1024,1024, "%s, %d: %s", inputFileName, index, str); // write matchedLine to shared memory
                    fprintf(outputFileTool, "%s", shared_memory + i * 1024); // migrate shared memory to output file
                    sem_post(semaphore);
                }
                index++;
            }
            fclose(inputFileTool);
            exit(0);
        }
    }

    for (int i = 0; i < n; i++) { // parent process waits all children
        wait(NULL);
    }
    fclose(outputFileTool);

    munmap(shared_memory, shared_memory_size); // Clean shared memory and semaphores
    sem_close(semaphore);
    return 0;
}


