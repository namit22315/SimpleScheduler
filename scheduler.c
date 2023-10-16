#include <stdio.h>
#include "dummy_main.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <sys/resource.h>
#include <semaphore.h>


#define MAX_NUM_PROCESSES 10
#define TIME_QUANTUM 2
#define COMMAND_TIMEOUT 60 // Set an appropriate timeout value (in seconds)

u_int16_t abc(int num_executed, int num_processes, int pids[MAX_NUM_PROCESSES]) {
    int vb = 0;
    while (num_executed < num_processes) {
        printf("nam %d  ", vb);
        vb = vb + 1;
        sleep(1);
    }
    return 0;
}

void execute_command(const char *cmd) {
    printf("Executing process with command: %s\n", cmd);
    execl("/bin/sh", "sh", "-c", cmd, NULL);
    perror("execl");
    exit(1);
}

void handle_timeout(int signum) {
    printf("Child process timed out. Terminating...\n");
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 53) {
        fprintf(stderr, "Usage: %s <NCPU> <TSLICE in seconds>\n", argv[0]);
        exit(1);
    }

    int NCPU = atoi(argv[1]);
    int TSLICE = atoi(argv[2]);
    //char *cmd_pointr = argv[3];
    //printf("\n_______________%s________\n", argv[9]);
    int g = 0;
    int ctr = 0;
    char *commands[50];
    for (int i = 0; i < 50; i++) {
        if (strcmp(argv[i + 3], " ") != 0) {
            commands[ctr] = argv[i + 3];
            ctr++;
            // printf("%d\n", ctr);
        }
    }
    sleep(1);
    int num_processes = ctr;
    int pids[MAX_NUM_PROCESSES];
    int jh;
    // printf("numproc %d \n",num_processes);

    int count = 0;
    // Initialize semaphore with NCPU permits
    int k = 0;
    while (g < num_processes) {
        // printf("start of while loop");
        sleep(0.2);
        if (NCPU > (num_processes-count)) {
            jh = num_processes;
        } else {
            jh = count + NCPU;
        }

        int num_executed = 0;
        int yht;
        sem_t semaphore;
        if (num_processes-count < NCPU) {
            yht = num_processes;
        }
        else{
            yht=NCPU;
        }
        sem_init(&semaphore, 0, yht);
        int flag =0;
        int gfd=0;
        for (int i = count; i < jh; i++) {
            // Acquire a permit, blocking if there are already NCPU processes running
            pid_t child_pid = fork();
            if (commands[i] == NULL) {
                //printf("\nrtdfaaergegaerge\n");
            } else {
                sem_wait(&semaphore);
                if (child_pid == -1) {
                    perror("fork");
                    exit(1);
                }
                if (child_pid == 0) {
                    // Child process
                    // Set an alarm to handle command timeouts
                    
                    signal(SIGALRM, handle_timeout);
                    alarm(COMMAND_TIMEOUT);
                    execute_command(commands[i]);
                    sleep(1);
                    
                    // pids[i] = child_pid;
                    // while(1){
                    // if (kill(pids[i], 0) == 0){
                    //     printf("sebhftx5njftnc gfgfxtjdttjfxthjgtfjf");
                    //     int child_status;
                    //     pid_t pid = waitpid(pids[i], &child_status, WNOHANG);
                    //     if (pid == pids[i]) {
                    //         printf("Process %d finished execution\n", i);
                    //         pids[i] = 0; // Mark the process as executed
                    //         num_executed++;
                    //         // j++;
                    //         flag++;
                    //         printf("namit315");
                    //         sem_post(&semaphore); // Release the permit
                    //         break;
                    //     }
                    // // }
                    // }
                } else {
                    // Parent process
                    gfd++;  
                     pids[i] = child_pid;
                }
                k++;
                if (k == NCPU) {
                    k = 0;
                }
            }
        }
        if (flag==jh){
            printf("ended all tasks successfully");
        }
        // sleep(10);
        if (k == NCPU) {
            k = 0;
        }
        int xz=0;
        char command_buffer[256];
        int j = 0;
        int bv=0;
        //printf("namit jain pro");
        while (1) {
            for (int i = count; i < jh; i++) {
                if (pids[i] > 0) {

                    if (kill(pids[i], 0) == 0) {

                        sleep(0.1);
                        int child_status;
                        pid_t pid = waitpid(pids[i], &child_status, WNOHANG);
                                                    // printf("namit315");
                        if (pid == pids[i]) {
                            printf("Process %d finished execution\n", i);
                            pids[i] = 0; // Mark the process as executed
                            num_executed++;
                            j++;
                            bv++;
                            //printf("____________%d............%d_____________",bv,gfd);
                            sem_post(&semaphore); // Release the permit
                            if (bv==gfd){

                                xz=1;
                                for (int b=0;b<bv;b++){
                                    commands[b+count]="\0";  
                                }
                                break;
                            }
                        }
                    }
                }
            }
            if (xz ==1){
                break;
            }
            // int flag = 0;
            // for (int i = 0; i < jh; i++) {
            //     if (commands[i] == NULL) {
            //         flag++;
            //     }
            //     if (flag == num_executed) {
            //         printf("all done");
            //         exit(0);
            //     }
            // }
        }
        sem_destroy(&semaphore);
        sleep(2);
        if(xz==1){
                printf("\nLeftover commands:\n");
                        for (int i = 0; i < num_processes; i++) {
            printf("command table %d: %s\n", i, commands[i]);
        }
        }
        
        // for (int i = count; i < jh; i++) {
        //     // if (pids[i] > 0) {
        //     //     // Check if a process has been running for too long and terminate it
        //     //     int status;
        //     //     if (waitpid(pids[i], &status, WNOHANG) == 0) {
        //     //         kill(pids[i], SIGTERM);
        //     //     } else {
        //     //         commands[i] = "\0";
        //     //     }
        //     // }
        // }

        sleep(TSLICE);

        g = g + yht;
        count = count + yht;
//        printf("end of while loop");
        if (num_processes==num_executed){
//            printf("dsjhfjesghsbrdj");
            break;
        }

        sleep(3);
    }
//    printf("\n\n\nhhrthttnghnty\n\n\n");

    return 0;
}