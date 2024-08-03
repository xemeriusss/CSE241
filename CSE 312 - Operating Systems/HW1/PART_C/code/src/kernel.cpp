
#include <common/types.h>
#include <gdt.h>
#include <memorymanagement.h>
#include <hardwarecommunication/interrupts.h>
#include <syscalls.h>
#include <hardwarecommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <drivers/ata.h>
#include <gui/desktop.h>
#include <gui/window.h>
#include <multitasking.h>

#include <drivers/amd_am79c973.h>


// #define GRAPHICSMODE


using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::gui;



void printf(char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x=0,y=0;

    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if(x >= 80)
        {
            x = 0;
            y++;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}
void printfHex16(uint16_t key)
{
    printfHex((key >> 8) & 0xFF);
    printfHex( key & 0xFF);
}
void printfHex32(uint32_t key)
{
    printfHex((key >> 24) & 0xFF);
    printfHex((key >> 16) & 0xFF);
    printfHex((key >> 8) & 0xFF);
    printfHex( key & 0xFF);
}


char* itoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

void printInt(int num){
    char numberStr[10];
    itoa(num,numberStr,10);
    printf(numberStr);
}






class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        char* foo = " ";
        foo[0] = c;
        printf(foo);
    }
};

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
public:
    
    MouseToConsole()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);        
    }
    
    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }
    
};




void sysprintf(char* str)
{
    asm("int $0x80" : : "a" (4), "b" (str));
}

// Make with syscalls
void taskA()
{
    while(true){
        for(int i = 0; i < 100000; i++){}
        printf("A");
    }

}

void taskB()
{
    while(true){
        for(int i = 0; i < 100000; i++){}    
        printf("B");
    }
    
}

void taskA_and_taskB(){

    int pid = getpid();
    int new_pid = fork(pid);

    if(new_pid == 0){
        taskA();
        exitt();
    }

    int new_pid2 = fork(pid);
    if(new_pid2 == 0){
        taskB();
        exitt();
    }

    waitpidd(new_pid);
    waitpidd(new_pid2);


    //exitt();
}

void collatz(int n) {

    printf("Collatz sequence for ");
    printInt(n);
    printf(" : ");
    while (n != 1) {
        if (n % 2 == 0) {
            n /= 2;
        }
        else {
            n = (3 * n) + 1;
        }
        printInt(n);

        printf(" ");
    }

    printf("\n");
}

void taskCollatz() {
    int n = 15;
    collatz(n);

    // int pid = getpid();
    // printf("TaskCollatz Pid: ");
    // printInt(pid);
    // printf("\n");

    exitt();
}

common::uint32_t long_running_program(int n) {
    int result = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result += i * j;
        }
    }
    return result;
}


common::uint32_t binary_search_program(int arr[], int l, int r, int x) {
    if (r >= l) {
        int mid = l + (r - l) / 2;

        if (arr[mid] == x) {
            return mid;
        }

        if (arr[mid] > x) {
            return binary_search_program(arr, l, mid - 1, x);
        }

        return binary_search_program(arr, mid + 1, r, x);
    }

    return -1;
}

void taskBinarySearch() {
    int arr[] = {2, 3, 4, 10, 40};
    int n = sizeof(arr) / sizeof(arr[0]);
    int x = 40;

    int result = binary_search_program(arr, 0, n - 1, x);
    if (result == -1) {
        printf("Element is not present in array\n");
    } else {
        printf("Binary Search: Element is at index ");
        printInt(result);
        printf("\n");
    }
}

common::uint32_t linear_search_program(int arr[], int n, int x) {
    for (int i = 0; i < n; ++i) {
        if (arr[i] == x) {
            return i;
        }
    }

    return -1;
}

void taskLinearSearch() {
    int arr[] = {2, 3, 4, 10, 40};
    int n = sizeof(arr) / sizeof(arr[0]);
    int x = 10;

    int result = linear_search_program(arr, n, x);
    if (result == -1) {
        printf("Element is not present in array\n");
    } else {
        printf("Linear Search: Element is at index: ");
        printInt(result);
        printf("\n");
    }
}

common::int32_t generateRandomNumber(int min, int max)
{
    uint64_t counter;
    int32_t num;

    // Get the clock counter
    asm volatile("rdtsc" : "=A"(counter));

    // Generate a random number using the clock counter
    counter = counter * 1103515245 + 12345;
    num = (int32_t)((counter >> 16) & 0x7FFFFFFF) % (max - min);

    if (num < 0)
        num += max;

    return num + min;
}


// =================================================================================================


void infiniteTask() {
    while (true) {
        //printf("C");
    }
}

void initProcess_fork() {

    int pid = getpid();

    int new_pid = fork(pid);
    if(new_pid == 0) {
        common::uint32_t pid = getpid();
        printf("child PID: ");
        printInt(pid);
        printf("\n");

        
        taskCollatz();
        //infiniteTask();

        exitt();

    } else {
        waitpidd(new_pid);
        printf("Parent process\n");
        
        // common::uint32_t pid = getpid();
        // printf("parent PID: ");
        // printfHex32(pid);
        // printf("\n");
    }

    int new_pid2 = fork(pid);
    if(new_pid2 == 0) {
        common::uint32_t pid = getpid();
        printf("child2 PID: ");
        printInt(pid);
        printf("\n");

        taskCollatz();
        //infiniteTask();

        exitt();

    } else {
        waitpidd(new_pid2);
        printf("Parent process2\n");
        
        // common::uint32_t pid = getpid();
        // printf("parent2 PID: ");
        // printfHex32(pid);
        // printf("\n");
    }

    // int new_pid3 = fork(pid);
    // if(new_pid3 == 0) {
    //     common::uint32_t pid = getpid();
    //     printf("child3 PID: ");
    //     printInt(pid);
    //     printf("\n");

    //     taskCollatz();

    // } else {
    //     waitpidd(new_pid3);
    //     printf("Parent process3\n");
        
    //     // common::uint32_t pid = getpid();
    //     // printf("parent3 PID: ");
    //     // printfHex32(pid);
    //     // printf("\n");
    // }

    // waitpidd(new_pid);
    // waitpidd(new_pid2);
    exitt();
}

void testFork() {
    common::uint32_t processPid = getpid();
    printf("initial process PID: ");
    printfHex32(processPid);
    printf("\n");

    int pid = fork(processPid);
    if (pid == 0) {
        // Child process after first fork
        common::uint32_t childPid = getpid();
        printf("child process should be 1: ");
        printfHex32(childPid);
        printf("\n");

        // Additional fork by the child process
        int anotherForkPid = fork(childPid);
        if (anotherForkPid == 0) {
            common::uint32_t anotherChildPid = getpid();
            printf("another child process should be 3: ");
            printfHex32(anotherChildPid);
            printf("\n");
        } else {
            printf("child process after second fork, should be 1: ");
            printfHex32(childPid);
            printf("\n");
        }
    } else {
        // Parent process after first fork
        common::uint32_t parentPid = getpid();
        printf("parent process should be 0: ");
        printfHex32(parentPid);
        printf("\n");

        // Additional fork by the parent process
        int additionalForkPid = fork(parentPid);
        if (additionalForkPid == 0) {
            common::uint32_t additionalChildPid = getpid();
            printf("additional child process should be 2: ");
            printfHex32(additionalChildPid);
            printf("\n");
        } else {
            printf("parent process after second fork, should be 0: ");
            printfHex32(parentPid);
            printf("\n");
        }
    }
}

void testWaitpidd(){

    common::uint32_t processPid = getpid();
    int pid = fork(processPid);

    if(pid == 0){
        for(int i = 0; i < 10000; i++){
            if(i%100==0){
                printInt(i);
                printf(" ");
            }
        }
        printf("\n");
        exitt();
    }else{
        
        waitpidd(pid);

        printf("Parent process\n");
    }

    exitt();
}

void test_fork_waitpid() {
    // printf("Starting the test of forking ten processes...\n");

    int parentpid = getpid();
    int loop_time = 2;

    printf("Parent process PID: ");
    printInt(parentpid);

    for (int i = 0; i < loop_time; i++) {
        common::uint32_t pid = fork(parentpid);
        if (pid == 0) { // Child process


            // taskCollatz();

            common::uint32_t res;
            res = long_running_program(1000);
            printInt(res);
            
            
            exitt();
        }
        // else{
        //     waitpidd(pid);
        // }
    }

    // The parent process waits for all child processes to complete
    for (int i = 0; i < loop_time; i++) {
        waitpidd(i+2);
        //printf("\n");
    }
    //printf("all child process done\n");
    exitt();
}


void printProgramNumber() {
    for (int i = 0; i < 10; i++) {
        printInt(i);
        printf(" ");
    }
    printf("\n");
    exitt();
}

void testExec() {
    int childpid;
    int parentpid = getpid();
    int pid = fork(parentpid);

    if (pid == 0) {
        childpid = getpid();
        // printf("child process PID: ");
        // printInt(childpid);
        // printf("\n");
        execc(taskCollatz);
        printf("exec failed\n");
    } else {
        // Wait for the child process to complete
        waitpidd(pid);
        printf("parent process\n");
    }

    exitt();
}

void lifeCycleA() {
    
    int parentpid = getpid();
    int loop_time = 3;
    int wait_time = 6;

    for (int i = 0; i < loop_time; i++) {

        common::uint32_t pid = fork(parentpid);
        if (pid == 0) { // Child process

            taskCollatz();
            
            exitt();
        }
    }

    for (int i = 0; i < loop_time; i++) {

        common::uint32_t pid = fork(parentpid);
        if (pid == 0) { // Child process

            common::uint32_t res;
            res = long_running_program(2000);
            printInt(res);
            printf("\n");
            
            exitt();
        }
    }

    // The parent process waits for all child processes to complete
    for (int i = 0; i < wait_time; i++) {
        waitpidd(i+2);
        //printf("\n");
    }

    exitt();
}

void FirstStrategy() {

    // First strategy: Randomly select a task and run it 10 times
    common::uint32_t pid1 = 0;
    int parentpid = getpid();
    int loop_time = 10;

    int randomNumber = generateRandomNumber(1,4);

    printf("Random Number: ");
    printInt(randomNumber);
    printf("\n");

    for(int i=0; i < loop_time; i++){

        // printf("Loop: ");
        // printInt(i+1);  
        // printf("\n");

        switch (randomNumber)
        {
            case 1:
                pid1 = fork(parentpid);
                if(pid1 == 0){
                    taskCollatz();
                    exitt();
                }
                break;
            case 2:
                pid1 = fork(parentpid);
                if(pid1 == 0){
                    taskBinarySearch();
                    exitt();
                }
                break;
            case 3:
                pid1 = fork(parentpid);
                if(pid1 == 0){
                    taskLinearSearch();
                    exitt();
                }
                break;
            case 4:
                pid1 = fork(parentpid);
                if(pid1 == 0){
                    common::uint32_t res;
                    res = long_running_program(1000);
                    printInt(res);
                    exitt();
                }
                break;
            default:
                break;
        }
    }

    // The parent process waits for all child processes to complete
    for (int i = 0; i < loop_time; i++) {
        waitpidd(i+2);
        //printf("\n");
    }

    exitt();
}

void SecondStrategy() {

    // Second strategy: Choosing 2 out 4 programs randomly and loading each program 3 times
    int randomNumber1 = generateRandomNumber(1,4);
    int randomNumber2 = generateRandomNumber(1,4);

    while(randomNumber1==randomNumber2){
        randomNumber2 = generateRandomNumber(1,4); // Make sure that the two random numbers are different
    }

    common::uint32_t pid2=0;
    common::uint32_t pid3=0;

    int parentpid = getpid();

    printf("Random Number: ");
    printInt(randomNumber1);
    printf(" ");
    printInt(randomNumber2);
    printf("\n");

    for(int i = 0; i < 3; i++){
        switch (randomNumber1)
        {
            case 1:
                pid2 = fork(parentpid);
                if(pid2 == 0){
                    taskCollatz();
                    exitt();
                }
                break;
            case 2:
                pid2 = fork(parentpid);
                if(pid2 == 0){
                    taskBinarySearch();
                    exitt();
                }
                break;
            case 3:
                pid2 = fork(parentpid);
                if(pid2 == 0){
                    taskLinearSearch();
                    exitt();
                }
                break;
            case 4:
                pid2 = fork(parentpid);
                if(pid2 == 0){
                    common::uint32_t res;
                    res = long_running_program(1000);
                    printInt(res);
                    exitt();
                }
                break;
            default:
                break;
        }
    }

    for(int i = 0; i < 3; i++){
        switch (randomNumber2)
        {
            case 1:
                pid3 = fork(parentpid);
                if(pid3 == 0){
                    taskCollatz();
                    exitt();
                }
                break;
            case 2:
                pid3 = fork(parentpid);
                if(pid3 == 0){
                    taskBinarySearch();
                    exitt();
                }
                break;
            case 3:
                pid3 = fork(parentpid);
                if(pid3 == 0){
                    taskLinearSearch();
                    exitt();
                }
                break;
            case 4:
                pid3 = fork(parentpid);
                if(pid3 == 0){
                    common::uint32_t res;
                    res = long_running_program(1000);
                    printInt(res);
                    exitt();
                }
                break;
            default:
                break;
        }
    }

    // The parent process waits for all child processes to complete
    for (int i = 0; i < 6; i++) {
        waitpidd(i+2);
        //printf("\n");
    }

    exitt();
}


typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}



extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
    printf("Hello World! --- http://www.AlgorithMan.de\n");

    GlobalDescriptorTable gdt;
    TaskManager taskManager(&gdt);
    

    Task task3(&gdt, taskA_and_taskB);
    taskManager.AddTask(&task3);

    
    InterruptManager interrupts(0x20, &gdt, &taskManager);
    SyscallHandler syscalls(&interrupts, 0x80);

    printf("Initializing Hardware, Keyboard\n");
    Desktop desktop(320,200, 0x00,0x00,0xA8);
    KeyboardDriver keyboard(&interrupts, &desktop);

    // printf("Initializing Hardware, Mouse\n");
    // Desktop desktop(320,200, 0x00,0x00,0xA8);
    // MouseDriver mouse(&interrupts, &desktop);
    
    // DriverManager drvManager;
    
    //     #ifdef GRAPHICSMODE
    //         KeyboardDriver keyboard(&interrupts, &desktop);
    //     #else
    //         PrintfKeyboardEventHandler kbhandler;
    //         KeyboardDriver keyboard(&interrupts, &kbhandler);
    //     #endif
    //     drvManager.AddDriver(&keyboard);
        
    
    //     #ifdef GRAPHICSMODE
    //         MouseDriver mouse(&interrupts, &desktop);
    //     #else
    //         MouseToConsole mousehandler;
    //         MouseDriver mouse(&interrupts, &mousehandler);
    //     #endif
    //     drvManager.AddDriver(&mouse);

    interrupts.Activate();


    while(1)
    {
        #ifdef GRAPHICSMODE
            desktop.Draw(&vga);
        #endif
    }
}
