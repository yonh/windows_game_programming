// defines for windows 
#define WINDOW_CLASS_NAME "WINCLASS1"
#define WINDOW_WIDTH  400
#define WINDOW_HEIGHT 300

// MACROS /////////////////////////////////////////////////
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

