#include <iostream>
#include <sys/ioctl.h>

#include "get_info.h"
#include "widgets.h"


int main (int argc, char **argv)
{
    	struct winsize w;
	constexpr int factor1 = 1024;
	constexpr int factor2 = 100;
	PC_info PC;
	Label static_info ("\n" + PC.Machine_Hostname + "\n" + PC.Kernel_Version + "\n" + PC.Kernel_compile_date + "\n" + PC.OS_type + "\n" + PC.CPU_name , 53 , 0 );
	char key;

	while(1){
		sleep_(1000);
	    	system("clear");

    		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
		//printf ("lines %d\n", w.ws_row);
    		//printf ("columns %d\n", w.ws_col);
		
		if(kbhit()){
			key = getchar();
			std::cout << key << std::endl;
			}

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

		//Variable info_________________________________________________________________________
		std::cout << s_format("Uptime         : ", GRN, H_DEFAULT) << x6.first << " hours  " << x6.second << " minutes" << "\n";
		std::cout << s_format("RAM            : " , GRN, H_BLK)<< x1.first << x1.second << "b used out of ";
		std::cout << x11.first << x11.second << "b \n";
		std::cout << s_format("CPU Usage      : ", GRN, H_DEFAULT)<< x2 << "% \n";
		std::cout << s_format("CPU Temp       : " , GRN, H_BLK)<< x3 << "°C \n";
		std::cout << s_format("Main Filesystem: " , GRN, H_DEFAULT)<< x4.first << x4.second << "b free of " << x45.first << x45.second  << "b \n";
		std::cout << s_format("Network Data   : " , GRN, H_BLK)<< x51.first << x51.second << "b/s received   " << x52.first << x52.second << "b/s sent" << "\n";
		std::cout << s_format("                 Total ⬇ " , GRN, H_DEFAULT)<< x7.first << x7.second << "b  " << s_format("    Total ⬆ " , GRN, H_DEFAULT)<< x8.first << x8.second << "b" << std::endl;

		if ( w.ws_col<100 && w.ws_row<13 ){
			// does / displays nothing
			}else if(w.ws_col<100){
				static_info.y=8;
				static_info.x=0;
				static_info.display();
			}else{
				static_info.y=0;
				static_info.x=53;
				static_info.display();
			}
	}
    return 0;

}
