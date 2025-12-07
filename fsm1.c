#include <windows.h>
#include <stdbool.h>
#include <stdio.h>


typedef enum {
    idle_state,
    start_state,
    run_state,
    termination_state
} TaskState;


static TaskState current_state = idle_state;
static bool first_call_after_abort = true;
static int volatile counter = 0;


void start(int count);
int run(void);
void termination(void);


__declspec(dllexport) int __stdcall tasks(int sig, int count)
{
    if (first_call_after_abort) {
        counter = 0;
        current_state = idle_state;
        first_call_after_abort = false;
    }
    switch (current_state)
    {
        case idle_state:
            if (sig == 2) {
                start(count);
                current_state = start_state;
            }
            break;

        case start_state:
            if (sig == 2) {
                // Start completed → go to run state
                run();
                current_state = run_state;
            } else {
                current_state = idle_state;
            }
            break;

        case run_state:
            if (sig == 2) {
                run();
            } else {
                // Stop received
                termination();
                current_state = termination_state;
            }
            break;

        case termination_state:
            // After termination, always return to idle
            current_state = idle_state;
            break;
    }

    
    return counter;
}


void start(int count)
{
    counter = count;   // initialize counter
}

int run(void)
{
    return counter++;  // increment each iteration
}

void termination(void)// for the soft stop 
{   
    counter=0;
    
    
}
__declspec(dllexport) int __stdcall AbortCleanup(void)// this is for the hard cleanup
{
    
    counter = 100;
    first_call_after_abort = true;
     char msg[256];
    sprintf(msg, "termination task excuted : %d", counter);

    // Show popup message
    MessageBoxA(
        NULL,
        msg,
        "DLL Abort Callback",
        MB_OK | MB_ICONINFORMATION | MB_TOPMOST
    );

}