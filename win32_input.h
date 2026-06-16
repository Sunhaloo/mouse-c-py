// C header definition
#ifndef WIN32_INPUT_H
#define WIN32_INPUT_H

// shared state ==> read by gui.c, written by win32_input.c
extern volatile int g_is_logging;
extern volatile double g_elapsed_time;
extern volatile float g_logging_duration;
extern volatile int g_win32_ready;

// function definitions / signatures for `win32_input.c` file

// call once at program start
int win32_setup(void);

// call when 'Start' button is pressed
void win32_start_logging(void);

// call when 'Stop' button is pressed
void win32_stop_logging(void);

// call after logging finishes naturally --> launches graphs.exe
void win32_launch_graphs(void);

// call once at program end
void win32_cleanup(void);

// C header "end" definition
#endif
