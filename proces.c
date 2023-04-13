#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define MAX_LINE_LENGTH 1024

void print_processes() {
    // Open the ps command and read the output line by line
    FILE *fp;
    char line[MAX_LINE_LENGTH];
    fp = popen("ps aux", "r");
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        printf("%s", line);
    }
    pclose(fp);
}

void print_top() {
    // Open the top command and read the output line by line
    FILE *fp;
    char line[MAX_LINE_LENGTH];
    fp = popen("top -b -n1", "r");
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        printf("%s", line);
    }
    pclose(fp);
}

void print_lsof() {
    // Open the lsof command and read the output line by line
    FILE *fp;
    char line[MAX_LINE_LENGTH];
    fp = popen("lsof", "r");
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        printf("%s", line);
    }
    pclose(fp);
}

void kill_process(pid_t pid) {
    // Send a SIGTERM signal to the process
    if (kill(pid, SIGTERM) == -1) {
        perror("kill failed");
    }
}

int main() {
    // Print the list of processes, top output, and files accessed
    printf("Processes:\n");
    print_processes();
    printf("\nTop:\n");
    print_top();
    printf("\nFiles Accessed:\n");
    print_lsof();

    // Prompt the user to enter a PID to kill
    pid_t pid;
    printf("\nEnter PID to kill: ");
    scanf("%d", &pid);

    // Kill the process with the given PID
    kill_process(pid);

    return 0;
}
