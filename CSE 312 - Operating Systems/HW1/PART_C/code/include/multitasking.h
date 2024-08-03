 
#ifndef __MYOS__MULTITASKING_H
#define __MYOS__MULTITASKING_H

#include <common/types.h>
#include <gdt.h>

namespace myos
{

    namespace hardwarecommunication
    {
        class InterruptHandler;
    }
    
    // CPUState is the structure to store the state of the CPU
    struct CPUState
    {
        // we push these registers to the stack in interrupt handler
        common::uint32_t eax; // accumulator
        common::uint32_t ebx; // base
        common::uint32_t ecx; // counter
        common::uint32_t edx; // data

        common::uint32_t esi; // stack index
        common::uint32_t edi; // data index
        common::uint32_t ebp; // base pointer

        /*
        common::uint32_t gs;
        common::uint32_t fs;
        common::uint32_t es;
        common::uint32_t ds;
        */
       // pushed by the processor automatically
        common::uint32_t error; // error code

        common::uint32_t eip; // instruction pointer
        common::uint32_t cs; // code segment
        common::uint32_t eflags; // flags
        common::uint32_t esp; // stack pointer
        common::uint32_t ss; // stack segment         
    } __attribute__((packed));
    
    enum TaskState
    {
        RUNNING,
        READY,
        TERMINATED, 
        WAITING
    };

    // Task is the structure to store the state of the task
    class Task
    {
    friend class TaskManager;
    private:
        common::uint8_t stack[4096]; // 4 KiB
        CPUState* cpustate;

        // Counter for the number of tasks
        static common::uint32_t pidCounter;

        // pid
        common::uint32_t pid = 0;

        // parent pid
        common::uint32_t ppid = 0;

        // state of the task
        TaskState state;

        common::uint32_t waitpid;

    public:
        Task(GlobalDescriptorTable *gdt, void entrypoint()); // entrypoint is the function pointer to the task
        Task();

        // // Task with priority
        // Task(GlobalDescriptorTable *gdt, void entrypoint(), common::uint32_t priority);

        // Get pid
        common::uint32_t getTaskPid();

        ~Task();
    };
    
    // TaskManager is the structure to manage the tasks
    class TaskManager
    {
    friend class hardwarecommunication::InterruptHandler;
    private:
        // Task* tasks[256]; 
        Task tasks[256];
        int numTasks;
        int currentTask;

        GlobalDescriptorTable* gdt = nullptr;

        // Get the current task
        int GetTask(common::uint32_t pid);
    
    // Protected because we don't want to expose these functions to the user
    protected:
            void PrintProcessTable();
            //common::uint32_t AddTask(void entrypoint());
            common::uint32_t ExecTask(void entrypoint());
            // common::uint32_t GetPid();
            common::uint32_t ForkTask(CPUState* cpustate);
            bool ExitCurrentTask();
            bool WaitTask(common::uint32_t pid);

    public:
        TaskManager(GlobalDescriptorTable* gdt); // Constructor to initialize the task manager

        TaskManager();
        ~TaskManager();
        bool AddTask(Task* task);
        CPUState* Schedule(CPUState* cpustate);
        // CPUState* PriorityBasedSchedule(CPUState* cpustate);

        common::uint32_t GetPid();
    };
    
    
    
}


#endif