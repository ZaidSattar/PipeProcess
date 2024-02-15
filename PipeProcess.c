#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void pushBack(int useWrite[2], int useRead[2], int firstValue, int secondValue, int *result, char *role, pid_t) {
    if (role[0] == 'P') { // This will be the parent process. Check if role[0] is 'P'
        printf("Parent (PID %d): Sending %d to child\n", getpid(), firstValue);
        write(useWrite[1], &firstValue, sizeof(firstValue));
        printf("Parent (PID %d): Sending %d to child\n", getpid(), secondValue);
        write(useWrite[1], &secondValue, sizeof(secondValue));
        read(useRead[0], result, sizeof(*result));
        printf("Parent (PID %d): Received %d from child\n", getpid(), *result);

    }
    else if (role[0] == 'C') { // This will be the child process, we will check if the role is C
        read(useRead[0], &firstValue, sizeof(firstValue));
        printf("\tChild (PID %d): Received %d from parent\n", getpid(), firstValue);
        read(useRead[0], &secondValue, sizeof(secondValue));
        printf("\tChild (PID %d): Received %d from parent\n", getpid(), secondValue);

        *result = firstValue * secondValue;
        printf("\tChild (PID %d): Sending %d to parent\n", getpid(), *result);
        write(useWrite[1], result, sizeof(*result));
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) { // this will check if there are 3 arguments in the command line
        fprintf(stderr, "Usage: %s <4-digit integer> <4-digit integer>\n", argv[0]); // if there is not 3 then it will do exit failure
        exit(EXIT_FAILURE);
    }

    int inputValue1 = atoi(argv[1]); // this will convert the integers in the command line arguments to strings
    int inputValue2 = atoi(argv[2]);
    printf("Your integers are %d %d\n", inputValue1, inputValue2); // this will print the converted integers

    int partialFirst1 = inputValue1 / 100, partialFirst2 = inputValue1 % 100, partialSecond1 = inputValue2 / 100, partialSecond2 = inputValue2 % 100;
    int pipeParentToChild[2], pipeChildToParent[2];
    if (pipe(pipeParentToChild) == -1 || pipe(pipeChildToParent) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t processID = fork(); // fork process
    if (processID == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (processID == 0) { // checks for process id value
        close(pipeParentToChild[1]);
        close(pipeChildToParent[0]);

        int calcResult;
        for (int i = 0; i < 4; ++i) {
            pushBack(pipeChildToParent, pipeParentToChild, 0, 0, &calcResult, "C", processID);
        }

        close(pipeParentToChild[0]);
        close(pipeChildToParent[1]);
        exit(EXIT_SUCCESS);
    } else {
        printf("Parent (PID %d): created child (PID %d)\n\n", getpid(), processID);
        close(pipeParentToChild[0]);
        close(pipeChildToParent[1]);

        int finalX, finalY1, finalY2, finalZ, resultX, resultY, resultZ;

        printf("###\n# Calculating X\n###\n");
        pushBack(pipeParentToChild, pipeChildToParent, partialFirst1, partialSecond1, &finalX, "P", processID);
        resultX = finalX * 10000;

        printf("\n###\n# Calculating Y\n###\n");
        pushBack(pipeParentToChild, pipeChildToParent, partialFirst1, partialSecond2, &finalY1, "P", processID);
        pushBack(pipeParentToChild, pipeChildToParent, partialFirst2, partialSecond1, &finalY2, "P", processID);
        resultY = (finalY1 + finalY2) * 100;

        printf("\n###\n# Calculating Z\n###\n");
        pushBack(pipeParentToChild, pipeChildToParent, partialFirst2, partialSecond2, &finalZ, "P", processID);
        resultZ = finalZ;

        wait(NULL);

        int finalResult = resultX + resultY + resultZ; // calculates the final result
        printf("\n%d*%d == %d + %d + %d == %d\n", inputValue1, inputValue2, resultX, resultY, resultZ, finalResult); //prints the final result

        close(pipeParentToChild[1]);
        close(pipeChildToParent[0]);
    }

    for x in
    return EXIT_SUCCESS;
}
