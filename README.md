# SimpleScheduler Program

The SimpleScheduler program is a component of the SimpleShell and is responsible for scheduling and managing the execution of user-submitted processes. This README provides details on the implementation and functionalities of the SimpleScheduler program.

## Implementation Details

### Basic Functionalities

1. *Starting SimpleShell*: To use SimpleScheduler, the user must first start the execution of the SimpleShell.

2. *Configuration Parameters*: During the launch of SimpleShell, the user provides two essential configuration parameters as command-line input:
    - NCPU: The total number of CPU resources available for executing processes.
    - TSLICE: The time quantum (in milliseconds) allocated for each process to run on a core.

3. *Interactive Shell*: SimpleShell operates in an infinite loop and displays a command prompt for user interaction. Users can submit their executable programs by entering commands. However, users are not allowed to submit programs that contain blocking calls (e.g., scanf, sleep) or programs with command-line parameters.

    
    SimpleShell$ submit ./a.out
    

4. *Submitting Processes*: The "submit" command creates a new process to run the executable program specified by the user.

5. *Process Control*: When a new process is created, it does not start execution immediately. Instead, it waits for a signal from the SimpleScheduler.

6. *SimpleScheduler Daemon*: The SimpleScheduler operates as a daemon with only one instance, regardless of the value of NCPU. It efficiently manages CPU scheduling tasks.

7. *Ready Queue*: SimpleScheduler maintains a ready queue, a data structure used to hold processes that are ready for execution. Processes are added to the rear of the queue and are dequeued in a round-robin fashion.

8. *Process Scheduling*: SimpleScheduler's main function is to schedule processes for execution. It takes NCPU processes from the front of the ready queue and signals them to start running. After the time quantum TSLICE expires, SimpleScheduler signals the running processes to stop their execution and adds them to the rear of the ready queue.

9. *Scheduling Quantum*: For simplicity, the scheduling quantum TSLICE begins and expires for all currently running jobs simultaneously, regardless of their arrival and termination times. If a new process arrives while some processes are already running, the new process will be scheduled to start running from the next TSLICE.

10. *Submit Multiple Jobs*: Users can submit any number of jobs at any given time. SimpleScheduler handles the scheduling of these jobs efficiently.

11. *Termination*: Terminating the SimpleShell will also terminate the SimpleScheduler. SimpleScheduler continues to work as designed until the natural termination of all user-submitted jobs.

12. *User Feedback*: Upon termination, SimpleShell prints the name, process ID (pid), execution time, and wait time for all jobs submitted by the user.

13. *Job Submission Restrictions*: Users of SimpleScheduler are not allowed to submit jobs using pipes, and processes should not contain any blocking calls.

For further information on the SimpleShell program and additional functionalities, please refer to the assignment requirements.

## Contributions

-    Namit Jain (2022315)
-    Round Robin Implmentation
-    Scheduler Linking
-    Array Creation
-    Semaphore Creation
-    Optimisation and Debugging

-    Pranav Tanwar (2022368)
-    Round Robin Implementation
-    Child Process Creation
-    History Management
-    Readme and comments handling
-    Optimization and debugging

## Github Link
https://github.com/namit22315/group_67_Assignment_3_OS