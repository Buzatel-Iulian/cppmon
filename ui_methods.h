#include <string>

//text
#define BLK "\033[30m"
#define RED "\033[31m"
#define GRN "\033[32m"
#define YEL "\033[33m"
#define BLU "\033[34m"
#define MAG "\033[35m"
#define CYN "\033[36m"
#define WHT "\033[37m"


//background/highlight
#define H_BLK "\033[40m"
#define H_RED "\033[41m"
#define H_GRN "\033[42m"
#define H_YEL "\033[43m"
#define H_BLU "\033[44m"
#define H_MAG "\033[45m"
#define H_CYN "\033[46m"
#define H_WHT "\033[47m"

#define DEFAULT "\033[39;49m"


void gotoxy(short x, short y) {
#ifdef _WIN32
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    printf("%c[%d;%df", 0x1B, y, x);
#endif
}

std::string s_format (std::string text, std::string f_color, std::string bg_color){

	return "blank ";
	}
