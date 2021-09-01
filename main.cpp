#include <iostream>
#include <sys/ioctl.h>

#include "get_info.h"
#include "widgets.h"


int main (int argc, char **argv)
{
    	//struct winsize w;
	constexpr int factor1 = 1024;
	constexpr int factor2 = 100;
	PC_info PC;

	while(1){
		//gotoxy(0,0);
		sleep(1);
	    	system("clear");

    		//ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
		//printf ("lines %d\n", w.ws_row);
    		//printf ("columns %d\n", w.ws_col);
		
		auto x1 =  auto_range(PC.get_memory_info(), convert_mode::bit_metric, factor2);
		auto x11 = auto_range(PC.total_RAM, convert_mode::bit_metric, factor2);
		auto x2 = digit_rm( PC.get_cpu_info() );
		int x3 = PC.get_temp_info();
		auto x4 = auto_range( PC.get_storage_info() );
		auto x45 = auto_range(PC.total_space);
		auto x5 = PC.get_network_info();
		auto x51 = auto_range(x5.first);
		auto x52 = auto_range(x5.second);
		auto x6 = PC.get_runtime_info();
		auto x7 = auto_range(PC.received);
		auto x8 = auto_range(PC.sent);

		//Static info___________________________________________________________________________
		std::cout << PC.CPU_name << std::endl;
		std::cout << "Machine_Hostname : " << PC.Machine_Hostname << std::endl;

		//Variable info_________________________________________________________________________
		std::cout << "Uptime: " << x6.first << " hours  " << x6.second << " minutes" << "\n";
		std::cout << "RAM: " << x1.first << x1.second << "b used out of ";
		std::cout << x11.first << x11.second << "b \n";
		std::cout << "CPU Usage: "<< x2 << "% \n";
		std::cout << "CPU Temp: " << x3 << "°C \n";
		std::cout << "Main Filesystem: " << x4.first << x4.second << "b free of " << x45.first << x45.second  << "b \n";
		std::cout << "Network Data: " << x51.first << x51.second << "b/s received   " << x52.first << x52.second << "b/s sent" << "\n";
		std::cout << "Total ⬇ " << x7.first << x7.second << "b  " << " Total ⬆ " << x8.first << x8.second << "b" << std::endl;
	}
    return 0;

}
