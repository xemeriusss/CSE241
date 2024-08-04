
#include <syscalls.h>
 
using namespace myos;
using namespace myos::common;
using namespace myos::hardwarecommunication;

void printInt(int num);


int myos::getpid()
{
    // eax is the syscall number
    int pid = -1;

    // 1 is the syscall number for getpid
    // int $0x80 is the interrupt instruction
    // c is the output operand to store the return value
    asm("int $0x80" : "=c" (pid) : "a" (1)); 

    return pid;
}

void myos::exitt() {
    asm("int $0x80" : : "a" (0)); // 0 corresponds to EXIT
}

int myos::fork(int pid) {
    // the value of the ecx register will be stored in the variable pid after the interrupt call completes
    asm ("int $0x80" : "=c" (pid) : "a" (2)); // 2 corresponds to FORK
}

void myos::waitpidd(int wait_pid) {
    asm ("int $0x80" : : "a" (3), "b" (wait_pid)); // 3 corresponds to WAITPID
}

int myos::execc(void entry_point()) {

    int res;
    asm("int $0x80" : "=c" (res) : "a" (4), "b" ((uint32_t)entry_point)); // 4 corresponds to EXEC
    return res;
}



 
SyscallHandler::SyscallHandler(InterruptManager* interruptManager, uint8_t InterruptNumber)
:    InterruptHandler(interruptManager, InterruptNumber  + interruptManager->HardwareInterruptOffset())
{
}

SyscallHandler::~SyscallHandler()
{
}


void printf(char*);

// When we make fork() syscall, we will create a new task
// It will continue from the same point where it left off. So we need to copy the esp

uint32_t SyscallHandler::HandleInterrupt(uint32_t esp)
{
    // Points start of the Cpustate struct
    CPUState* cpu = (CPUState*)esp;

    
    // eax is the syscall number
    switch(cpu->eax)
    {
        // case 4:
        //     // cpu->ebx: in CPUState, ebx is the pointer to the string
        //     printf((char*)cpu->ebx); 
        //     break;

        case 0:
            if(InterruptHandler::syscall_exit()) {
                return InterruptHandler::HandleInterrupt(esp);
            }
            break;

        case 1:
            cpu->ecx = InterruptHandler::syscall_getpid();
            break;

        case 2:
            cpu->ecx = InterruptHandler::syscall_fork(cpu);
            //return InterruptHandler::HandleInterrupt(esp); 
            break;

        case 3:
            if(InterruptHandler::syscall_waitpid(esp)) {
                return InterruptHandler::HandleInterrupt(esp);
            }
            break;

        case 4:
            esp = InterruptHandler::syscall_exec(cpu->ebx);
            break;
            
        default:
            break;
    }

    
    return esp;
}

