//Using the ugly \x1B instead of \e because apparently the latter is not really standard, so just in case someone wanted to compile this with something that isn't gcc/clang ...
#define COLOR_RESET  "\x1B[0m"
#define COLOR_RED  "\x1B[31m"
#define COLOR_GREEN  "\x1B[32m"
#define COLOR_YELLOW  "\x1B[33m"
#define COLOR_BLUE  "\x1B[34m"
#define COLOR_MAG  "\x1B[35m"
#define COLOR_CYAN  "\x1B[36m"
#define COLOR_WHITE  "\x1B[37m"