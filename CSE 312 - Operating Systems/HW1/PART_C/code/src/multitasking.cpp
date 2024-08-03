
#include <multitasking.h>

using namespace myos;
using namespace myos::common;

void printf(char* str);
void printfHex32(uint32_t key);
void printInt(int num);


// Initialize the pidCounter
common::uint32_t Task::pidCounter = 1;

// Constructor
Task::Task()
{
    state = TERMINATED;
    ppid = 0;

    cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));
    cpustate -> eax = 0;
    cpustate -> ebx = 0;
    cpustate -> ecx = 0;
    cpustate -> edx = 0;

    cpustate -> esi = 0;
    cpustate -> edi = 0;
    cpustate -> ebp = 0;
    cpustate -> eip = 0;
    cpustate -> cs = 0;
    cpustate -> eflags = 0x202;
}

// Constructor with entrypoint
Task::Task(GlobalDescriptorTable *gdt, void entrypoint())
{
    ppid = 0;
    pid = pidCounter++; // Increment the pidCounter and assign it to the pid
    waitpid = 0; // waitpid is the pid of the task that the current task is waiting for

    // start of stack + size of stack - size of CPUState
    // This way we get the beginning of the CPUState structure
    cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));
    
    // Initialize the CPUState registers
    cpustate -> eax = 0; // Accumulator register
    cpustate -> ebx = 0; // Base register
    cpustate -> ecx = 0; // Counter register
    cpustate -> edx = 0; // Data register

    cpustate -> esi = 0; // Stack index register
    cpustate -> edi = 0; // Data index register
    cpustate -> ebp = 0; // Stack base pointer register
    
    /*
    cpustate -> gs = 0;
    cpustate -> fs = 0;
    cpustate -> es = 0;
    cpustate -> ds = 0;
    */
    
    // cpustate -> error = 0;    
   
    // cpustate -> esp = ; // Stack pointer
    cpustate -> eip = (uint32_t)entrypoint; // Instruction pointer
    cpustate -> cs = gdt->CodeSegmentSelector(); // Code segment selector
    // cpustate -> ss = ;
    cpustate -> eflags = 0x202; // Flags register, 0x202 is the value of the flags register when the CPU is in kernel mode
    
}

Task::~Task()
{
}

// Get the pid of the task from the task
common::uint32_t Task::getTaskPid()
{
    return pid;
}

        
TaskManager::TaskManager()
{
    // Initialize the task manager
    numTasks = 0;
    currentTask = -1;
}

TaskManager::TaskManager(GlobalDescriptorTable *gdt)
{
    // Initialize the task manager
    numTasks = 0;
    currentTask = -1;
    this->gdt = gdt; // GlobalDescriptorTable is
}

TaskManager::~TaskManager()
{
}

bool TaskManager::AddTask(Task* task)
{
    if(numTasks >= 256)
        return false;

    // Copy the tasks stack to the new task
    tasks[numTasks].state = READY; 
    tasks[numTasks].pid = task->pid; 

    // start of stack + size of stack - size of CPUState
    // This way we get the beginning of the CPUState structure
    tasks[numTasks].cpustate = (CPUState*)(tasks[numTasks].stack + 4096 - sizeof(CPUState));
    
    // Copy the registers of the task to the new task
    tasks[numTasks].cpustate -> eax = task->cpustate->eax;
    tasks[numTasks].cpustate -> ebx = task->cpustate->ebx;
    tasks[numTasks].cpustate -> ecx = task->cpustate->ecx;
    tasks[numTasks].cpustate -> edx = task->cpustate->edx;

    tasks[numTasks].cpustate -> esi = task->cpustate->esi;
    tasks[numTasks].cpustate -> edi = task->cpustate->edi;
    tasks[numTasks].cpustate -> ebp = task->cpustate->ebp;
    
    tasks[numTasks].cpustate -> eip = task->cpustate->eip;
    tasks[numTasks].cpustate -> cs = task->cpustate->cs;
    
    tasks[numTasks].cpustate -> eflags = task->cpustate->eflags;
    numTasks++;

    return true;
}


// Get the current task pid from the task manager
common::uint32_t TaskManager::GetPid()
{
    if(currentTask < 0)
        return 0;
    return tasks[currentTask].pid;
}

// Get the current task pid
int TaskManager::GetTask(common::uint32_t givenPid)
{
    for(int i = 0; i < numTasks; i++)
    {
        if(tasks[i].pid == givenPid)
            return i;
    }
    return -1;
}

bool TaskManager::ExitCurrentTask()
{
    if(currentTask < 0)
        return false;
    tasks[currentTask].state = TERMINATED;

    return true;
}

common::uint32_t TaskManager::ForkTask(CPUState* cpustate)
{
    if(numTasks >= 256)
        return 0;

    tasks[numTasks].state = READY; 

    tasks[numTasks].ppid = tasks[currentTask].pid; // Parent pid is the pid of the current task

    tasks[numTasks].pid = Task::pidCounter++; // Increment the pidCounter and assign it to the pid

    // Copy the stack of the parent process to the child process
    for (int i = 0; i < sizeof(tasks[currentTask].stack); i++)
    {
        tasks[numTasks].stack[i] = tasks[currentTask].stack[i];
    }
    
    // Set the cpustate of the child process
    common::uint32_t currentTaskOffset = (common::uint32_t)cpustate - (common::uint32_t)tasks[currentTask].stack;
    tasks[numTasks].cpustate = (CPUState*)((common::uint32_t)tasks[numTasks].stack + currentTaskOffset);

    // Set the stack pointer(esp) of the child process
    common::uint32_t stackOffset = cpustate->esp - (common::uint32_t)tasks[currentTask].stack;
    tasks[numTasks].cpustate->esp = (common::uint32_t)tasks[numTasks].stack + stackOffset;

    tasks[numTasks].cpustate->ecx = 0; // Return value of the child process should be 0

    numTasks++;
    return tasks[numTasks-1].pid; // return new process id of the child process
}

bool TaskManager::WaitTask(common::uint32_t esp)
{
    CPUState *cpustate =  (CPUState*)esp;

    common::uint32_t givenPid = cpustate->ebx;

    // Prevent the current task from running
    if(tasks[currentTask].pid == givenPid || givenPid == 0)
    {
        // tasks[currentTask].state = TERMINATED;
        return false;
    }

    int taskIndex = GetTask(givenPid);
    if(taskIndex < 0)
    {
        return false;
    }

    // If number of tasks is less than the task index, return false
    if(numTasks <= taskIndex)
    {
        return false;
    }

    // If the task is terminated, return false
    if(tasks[taskIndex].state == TERMINATED)
    {
        return false;
    }

    tasks[currentTask].cpustate = cpustate; // Save the current cpustate
    tasks[currentTask].waitpid = givenPid; // Set the waitpid of the current task to the given pid
    tasks[currentTask].state = WAITING; // Set the state of the current task to WAITING

    return true;
}

common::uint32_t TaskManager::ExecTask(void entrypoint())
{
    // Set the entrypoint of the current task to the given entrypoint
    tasks[currentTask].state=READY;
    tasks[currentTask].cpustate = (CPUState*)(tasks[currentTask].stack + 4096 - sizeof(CPUState));
    
    tasks[currentTask].cpustate -> eax = 0;
    tasks[currentTask].cpustate -> ebx = 0;
    tasks[currentTask].cpustate -> ecx = tasks[currentTask].pid;
    tasks[currentTask].cpustate -> edx = 0;

    tasks[currentTask].cpustate -> esi = 0;
    tasks[currentTask].cpustate -> edi = 0;
    tasks[currentTask].cpustate -> ebp = 0;
    
    // We set the eip of the current task to the entrypoint. This way the task will start from the entrypoint.
    // Entry point is the function pointer to the task
    tasks[currentTask].cpustate -> eip = (uint32_t)entrypoint; 
    tasks[currentTask].cpustate -> cs = gdt->CodeSegmentSelector();
    tasks[currentTask].cpustate -> eflags = 0x202;

    return (uint32_t)tasks[currentTask].cpustate; // return the cpustate of the current task
}


void TaskManager::PrintProcessTable()
{
    // printf("Process Table\n");
    printf("Pid  Ppid  State: ");
    for(int i = 0; i < numTasks; i++)
    {
        printInt(tasks[i].pid);
        printf("    ");
        printInt(tasks[i].ppid);
        printf("    ");

        if(tasks[i].state == READY){
            if(i == currentTask)
                printf("RUNNING");
            else
                printf("READY");
        }
            
        else if(tasks[i].state == TERMINATED)
            printf("TERMINATED");
        else if(tasks[i].state == WAITING)
            printf("WAITING");

        printf("\n");
    }
}

// // Schedule with round-robin scheduling
CPUState* TaskManager::Schedule(CPUState* cpustate)
{
    
    // To slow down the scheduler
    for(int i = 0; i < 10000000; i++){
    }

    // If there are no tasks, return the current cpustate
    if(numTasks <= 0)
        return cpustate;

    // If we are already doing scheduling, we save the current cpustate
    if(currentTask >= 0)
        tasks[currentTask].cpustate = cpustate;

    // PrintProcessTable();

    // Compute the index of the next task using round-robin scheduling
    int nextTask = (currentTask + 1) % numTasks;

    while(tasks[nextTask].state != READY)
    {

        // If the next task is WAITING and it waits for a task
        if (tasks[nextTask].state == WAITING) {

            int waitedTaskIndex = GetTask(tasks[nextTask].waitpid);

            //Set the waiting task to READY if the task it waits on is TERMINATED
            if (tasks[waitedTaskIndex].state == TERMINATED)
            {
                tasks[nextTask].waitpid = 0;
                tasks[nextTask].state = READY;
                continue;
            }
            
            // Switch to the task that is waited for if it is READY
            if (tasks[waitedTaskIndex].state == READY)
            {
                nextTask = waitedTaskIndex;
                continue;
            }

        } 

        nextTask = (nextTask + 1) % numTasks;
    }

    currentTask = nextTask;

    return tasks[currentTask].cpustate; // return the new current task's cpustate

    // ==========================================================================

    // ONLY CHECK READY TASKS
    
    // // If there are no tasks, return the current cpustate
    // if(numTasks <= 0)
    //     return cpustate;

    // // If we are already doing scheduling, we save the current cpustate
    // if(currentTask >= 0)
    //     tasks[currentTask].cpustate = cpustate;

    // printf("\n");
    // printf("\n");
    // printf("\n");
    // PrintProcessTable();

    // // Compute the index of the next task using round-robin scheduling
    // int nextTask = (currentTask + 1) % numTasks;

    // while(tasks[nextTask].state != READY)
    // {
    //     nextTask = (nextTask + 1) % numTasks;
    // }

    // currentTask = nextTask;

    // // PrintProcessTable();


    // return tasks[currentTask].cpustate; // return the new current task's cpustate

}




    