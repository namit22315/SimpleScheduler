#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h> // for time tracking
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>


// Structure for history
struct CommandHistory {
    char commands[20][1200];
    int count;
};



// Structure to store PID, execution time, and duration
struct CommandHistory2 {
    pid_t pids[20];
    time_t execution_time[20];
    double execution_duration[20];
    int count;
};

// Array to store commands starting with "submit"
char dfg[50][1200];
int dfg_count = 0;

void remove_newline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

// Function to execute a single command and record its PID, execution time, and duration
char* execute_single_command(char *command, struct CommandHistory *history, struct CommandHistory2 *history2,int NCPU , int TSLICE) {
    time_t start_time;
    char* result = (char*)malloc(1200 + 1);  // +1 for null terminator

    if (strncmp(command,"dfg",3)==0){
        sem_t dfg_semaphore;
        sem_init(&dfg_semaphore,0,1);
        int fd;
        char (*dfg)[1200];

        // Open a temporary file to use for mmap
        //printf("hello");
        fd = open("/tmp/dfg_file", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        if (fd == -1) {
            perror("open");
            exit(1);
        }

        // Size of the memory mapping, based on the number of elements in dfg
        size_t mmap_size = 20 * sizeof(char[1200]);

        // Map the file into memory
        dfg = mmap(0, mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (dfg == MAP_FAILED) {
            perror("mmap");
            exit(1);
        }
        sem_wait(&dfg_semaphore);
        sem_post(&dfg_semaphore);
    }
    
    // Check if the command starts with "submit"

    // Check if the command ends with "&"
    int run_in_background = 0;
    int command_length = strlen(command);
    if (command_length > 1 && command[command_length - 1] == '&') {
        run_in_background = 1;
        // Remove the "&" from the command
        command[command_length - 1] = '\0';
    }

    // Add the command to history
if(strncmp(command,"letsgo",6)==0){
        if (history->count < 20) {
        strncpy(history->commands[history->count], command+7, 1200);
        history->count++;
    } else {
        // If the history is full, remove the oldest command
        for (int i = 0; i < 20 - 1; i++) {
            strncpy(history->commands[i], history->commands[i + 1], 1200);
        }
        strncpy(history->commands[20 - 1], command+7, 1200);
    }

    // Record the start time
    time_t start_time = time(NULL);
}
else{
        if (history->count < 20) {
        strncpy(history->commands[history->count], command, 1200);
        history->count++;
    } else {
        // If the history is full, remove the oldest command
        for (int i = 0; i < 20 - 1; i++) {
            strncpy(history->commands[i], history->commands[i + 1], 1200);
        }
        strncpy(history->commands[20 - 1], command, 1200);
    }

    // Record the start time
    time_t start_time = time(NULL);
}
    if (strncmp(command, "submit", 6) == 0) {
        if (dfg_count < 50) {
            strncpy(dfg[dfg_count], command + 7, 1200);
            dfg_count++;
        } else {
            // Handle array full scenario if needed
        }
        snprintf(result, 1200, "Command added to dfg array and not executed.\n");
        return result;
    }

    if (strncmp(command,"letsgo",6)==0){
        //printf("namit");
        char string_sched[100]; // Adjust the array size as needed

    // Format the string with the values of NCPU and TSLICE
    sprintf(string_sched, "./scheduler %d %d", NCPU, TSLICE);

    char ncu[20];   // Adjust the array size as needed
    char tsli[20];
    // char ptr_dfg[20];
    sprintf(ncu, "%d", NCPU);
    sprintf(tsli, "%d", TSLICE);
    // sprintf(ptr_dfg,"%d",&dfg);
    for (int i=dfg_count;i<50;i++){
        strncpy(dfg[i], " ", 1200);
    }

    pid_t pidd=fork();
  
        if (pidd==0){
        execl("./scheduler", "scheduler", ncu, tsli,dfg[0],dfg[1],
        dfg[2],dfg[3],dfg[4],dfg[5],dfg[6],dfg[7],dfg[8],dfg[9],
        dfg[10],dfg[11],dfg[12],dfg[13],dfg[14],dfg[15],dfg[16],
        dfg[17],dfg[18],dfg[19],dfg[20],dfg[21],dfg[22],dfg[23],
        dfg[24],dfg[25],dfg[26],dfg[27],dfg[28],dfg[29],dfg[30],
        dfg[31],dfg[32],dfg[33],dfg[34],dfg[35],dfg[36],dfg[37],
        dfg[38],dfg[39],dfg[40],dfg[41],dfg[42],dfg[43],dfg[44],
        dfg[45],dfg[46],dfg[47],dfg[48],dfg[49], (char *)NULL);

        }
        else{
            while(1){
        if (kill(pidd,0)==0){
                        for (int cc=0; cc<50; cc++){
                strncpy(dfg[cc]," ",1200);
            }
            dfg_count=0;

            
            
            break;
        }
}


        }
                    history2->pids[history2->count] = pidd;
            history2->execution_time[history2->count] = start_time;
            sleep(1);
                                time_t end_time = time(NULL);
                    double duration = difftime(end_time, start_time);
                    history2->execution_duration[history2->count] = duration;
                    sleep(1);
    return 0;
    }
    // Check if the command starts with '#!/bin/bash'
    if (strncmp(command, "#!/bin/bash", 11) == 0) {
        // This is a shebang line, which does not need to be executed directly
        // snprintf(result, 1200, "%s\tShebang line, not executed directly\n", command);
    } else{
        // Fork a child process to execute the command
        pid_t pid = fork();

        if (pid == -1) {
            printf("fork");
        } else if (pid == 0) {
            // Child process
            // Use the system shell to execute the command
            int exec_result = execl("/bin/sh", "sh", "-c", command, (char *)NULL);

            if (exec_result == -1) {
                printf("system failure");
                exit(EXIT_FAILURE);
            } else {
                exit(WEXITSTATUS(exec_result));
            }
        } else {
            // Parent process
            // Record the PID and execution start time
            history2->pids[history2->count] = pid;
            history2->execution_time[history2->count] = start_time;

            if (run_in_background == 0) {
                int status;
                waitpid(pid, &status, 0);

                if (!WIFEXITED(status)) {
                    snprintf(result, 1200, "%s\tCommand did not exit normally\n", command);
                } else {
                    // Record the execution duration
                    time_t end_time = time(NULL);
                    double duration = difftime(end_time, start_time);
                    history2->execution_duration[history2->count] = duration;

                    snprintf(result, 1200, "\n" );
                }
            } else {
                // If the command is running in the background, don't wait for it
                snprintf(result, 1200, "%s\tCommand running in the background with PID: %d\n", command, pid);
            }
        }
    }

    history2->count++;
    return result;
}

void execute_piped_commands(char *piped_commands, struct CommandHistory *history, struct CommandHistory2 *history2,int NCPU,int TSLICE) {
    // Split piped_commands into individual commands
    char *command;
    char *saveptr;
    command = strtok_r(piped_commands, "|", &saveptr);

    int pipe_fd[2];
    int prev_pipe_read = -1; // File descriptor for the previous pipe's read end

    while (command != NULL) {
        // Create a pipe
        if (pipe(pipe_fd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        // Fork a child process
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            // Close the write end of the current pipe
            close(pipe_fd[1]);

            // Redirect stdin to the previous pipe's read end
            if (prev_pipe_read != -1) {
                dup2(prev_pipe_read, 0);
                close(prev_pipe_read);
            }

            // Redirect stdout to the current pipe's write end
            dup2(pipe_fd[1], 1);

            // Close all unused file descriptors
            close(pipe_fd[0]);

            // Execute the command
            execute_single_command(command,history,history2,NCPU,TSLICE);

            exit(EXIT_SUCCESS);
        } else {
            // Parent process
            // Close the read end of the current pipe
            close(pipe_fd[1]);

            // Close the previous pipe's read end
            if (prev_pipe_read != -1) {
                close(prev_pipe_read);
            }

            // Wait for the child to finish
            int status;
            waitpid(pid, &status, 0);

            // Set the previous pipe's read end to the current pipe's read end
            prev_pipe_read = pipe_fd[0];

            command = strtok_r(NULL, "|", &saveptr);
        }
    }
}

void add_to_history(struct CommandHistory *history, char *command) {
    if (history->count < 20) {
        strncpy(history->commands[history->count], command, 1200);
        history->count++;
    } else {
        // If the history is full, remove the oldest command
        for (int i = 0; i < 20 - 1; i++) {
            strncpy(history->commands[i], history->commands[i + 1], 1200);
        }
        strncpy(history->commands[20 - 1], command, 1200);
    }
}

// Function to execute a shell script
void execute_shell_script(char *script_name, struct CommandHistory *history, struct CommandHistory2 *history2,int NCPU,int TSLICE) {
    FILE *script_file = fopen(script_name, "r");
    if (script_file) {
        char script_command[1200];

        while (fgets(script_command, sizeof(script_command), script_file) != NULL) {
            // Remove the trailing newline character
            remove_newline(script_command);

            // Execute the command from the script file
            char *result = execute_single_command(script_command, history, history2,NCPU,TSLICE);
            printf("%s", result);
            free(result);
        }

        fclose(script_file);
    } else {
        printf("Error: Unable to open script file '%s'\n", script_name);
    }
}

int main(int argc, char *argv[]) {
    if (argc!=3){
        fprintf(stderr, "Usage: %s <NCPU> <TSLICE in seconds>\n", argv[0]);
        exit(1);
    }
    char command[1200];
    int NCPU = atoi(argv[1]);
    int TSLICE = atoi(argv[2]);

    // Initialize the command history
    struct CommandHistory history;
    history.count = 0;
    char comand[1]="";
    // Initialize the CommandHistory2
    struct CommandHistory2 history2;
    history2.count = 0;

    do {
        printf("SimpleShell$ ");
        fgets(command, sizeof(command), stdin);
        remove_newline(command);

        if (strcmp(command, "exit") == 0) {
            // Display the CommandHistory2 (PID, execution time, duration)
            for (int i = 0; i < history2.count; i++) {
                printf("\nCommand %d:\n", i + 1);
                printf("%s \n",history.commands[i]);
                printf("PID: %d\n", history2.pids[i]);
                printf("Execution Time: %s", asctime(localtime(&history2.execution_time[i])));
                printf("Execution Duration: %.2f seconds\n", history2.execution_duration[i]);
            }
            exit(0);
            break;
             // Exit the shell
        }
        else if (strncmp(command, "letsgo", 6) == 0){
        
        char *result2 = execute_single_command(command+7, &history, &history2,NCPU,TSLICE);
        for(int f=0;f<dfg_count;f++){
        char *result = execute_single_command(command, &history, &history2,NCPU,TSLICE);
        free(result);
        }
        
        free(result2);

        }
        else if (strcmp(command, "dfg") == 0) {
            // Display the commands in the dfg array
            printf("Commands starting with 'submit' in dfg array:\n");
            for (int i = 0; i < dfg_count; i++) {
                printf("%d: %s\n", i + 1, dfg[i]);
            }
        }
        else if (strstr(command, ".sh") != NULL && strstr(command, "./")) {
            // The command contains ".sh", indicating a script file
            execute_shell_script(command, &history, &history2,NCPU,TSLICE);
        } else if (strcmp(command, "history") == 0) {
            // Display the command history
            printf("Command History:\n");
            for (int i = 0; i < history.count; i++) {
                printf("%d: %s\n", i + 1, history.commands[i]);
            }
        } else {
            // Execute the command and get the result
            if (strchr(comand, '|') != NULL) {
                execute_piped_commands(comand,&history,&history2,NCPU,TSLICE);
            } else {
                char *result = execute_single_command(command, &history, &history2,NCPU,TSLICE);
                printf("%s", result);
                free(result);
            }
            
        }
    } while (1);

    return 0;
}