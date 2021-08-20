#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 
#include <iomanip>  // std::seprecision, std::fixed 
#include <math.h>
#include <sys/statvfs.h>
#include <cstring>
#include <utility>
// See: https://man7.org/linux/man-pages/man2/sysinfo.2.html
#include <sys/sysinfo.h>



std::uint64_t sum (std::uint64_t val[9],int lim){
	std::uint64_t suma=0;
       	for(int i = 0; i<=lim; i++){
	suma+=val[i];
	}
	return suma;
}

double digit_rm (double val, int factor = 100){
	return (double)((int)(val*factor))/factor; 
	}

enum class convert_mode {up_metric, low_metric, metric, bit_metric, imperial};

std::pair < double, std::string > auto_range (uint64_t x, convert_mode mode = convert_mode::up_metric, int factor2 = 10){
	constexpr int factor11 = 1000;
	constexpr int factor12 = 1024;
	double val = (double) x;
	int i = 0;
	switch (mode){
		case convert_mode::up_metric:
			while (val >= factor11 ){
				val = val / factor11;
				i++;
				}
			switch (i) {
				case 0 : return std::make_pair( digit_rm( val, factor2 ) ,"_" ); 
				case 1 : return std::make_pair( digit_rm( val, factor2 ) ,"K" ); 
				case 2 : return std::make_pair( digit_rm( val, factor2 ) ,"M" ); 
				case 3 : return std::make_pair( digit_rm( val, factor2 ) ,"G" );
				case 4 : return std::make_pair( digit_rm( val, factor2 ) ,"T" );
				case 5 : return std::make_pair( digit_rm( val, factor2 ) ,"P" ); 
				}
		case convert_mode::bit_metric:
				val=val*factor12;
				while (val >= factor12 ){
				val = val / factor12;
				i++;
				}
			switch (i) {
				case 0 : return std::make_pair( digit_rm( val, factor2 ) ,"_" ); 
				case 1 : return std::make_pair( digit_rm( val, factor2 ) ,"K" ); 
				case 2 : return std::make_pair( digit_rm( val, factor2 ) ,"M" ); 
				case 3 : return std::make_pair( digit_rm( val, factor2 ) ,"G" );
				case 4 : return std::make_pair( digit_rm( val, factor2 ) ,"T" );
				case 5 : return std::make_pair( digit_rm( val, factor2 ) ,"P" ); 
				}
		default:return std::make_pair(0,"_");

		
	}
	return std::make_pair(0,"_");
	}


class PC_info{

	private:

	public:
	double total_RAM;
	std::string CPU_info, system_info;
	uint64_t total_space, received, sent;


	PC_info (){
		//get the total RAM memory_______________________________________________________________
		constexpr const char* meminfo_file 		= "/proc/meminfo";
	
		auto ifs = std::ifstream{meminfo_file};
		if(!ifs.good()){
			throw std::runtime_error("Error: unable to memory-info file.");
		}
		std::string line, label;
		std::uint64_t value; 
		while( std::getline(ifs, line) )
		{		
			std::stringstream ss{line};	
			ss >> label >> value;
			if(label == "MemTotal:"){
				total_RAM = value ;
			}else{
				continue;
			}
		}

		//get the toatal storage_________________________________________________________________
		struct statvfs fiData;
		statvfs(".",&fiData);
		total_space =  fiData.f_blocks * fiData.f_bsize;
		//get system info________________________________________________________________________

	}


double get_memory_info(){
	constexpr const char* meminfo_file 		= "/proc/meminfo";
	
	auto ifs = std::ifstream{meminfo_file};
	if(!ifs.good()){
		throw std::runtime_error("Error: unable to memory-info file.");
	}
	std::string line, label;
	std::uint64_t value, left = total_RAM ; 
	while( std::getline(ifs, line) )
	{		
		std::stringstream ss{line};	
		ss >> label >> value;
		if(label == "MemAvailable:"||label == "Buffer:"){
			left=left-value;
		}else{
			continue;
		}
	}
		return (left);
}

double get_cpu_info(){
	constexpr const char* cpuinfo_file 		= "/proc/stat";
	static std::uint64_t total_work_1 = 0,user_work_1 = 0,aux;
	auto ifs = std::ifstream{cpuinfo_file};
	if(!ifs.good()){
		throw std::runtime_error("Error: unable to cpu-info file.");
	}
	std::string line, label;
	std::uint64_t value[9],total_work_2,user_work_2;
	while( std::getline(ifs, line) )
	{		
		std::stringstream ss{line};	
		ss >> label;
		if(label == "cpu"){
			for(int i = 0; i<=9; i++){
				ss >> value[i];
			}
			total_work_2=sum(value,9);  	
			user_work_2=sum(value,2);   
			//std::cout << "\nCPU usage " << (double)(user_work_2-user_work_1)/(double)(total_work_2-total_work_1)*100 << "%" <<"\n";
			double aux1 = (double)(user_work_2-user_work_1)/(double)(total_work_2-total_work_1)*100; 
			total_work_1=total_work_2;
			user_work_1=user_work_2;
			
			return aux1;

		}else{
			continue;
		}

	}
	return 0;
}

int get_temp_info(){
	constexpr const char* tempinfo_file 		= "/sys/class/thermal/thermal_zone0/temp";
	auto ifs = std::ifstream{tempinfo_file};
	if(!ifs.good()){
		throw std::runtime_error("Error: unable to cpu-info file.");
	}
	std::string line;
	std::uint64_t value;
	while( std::getline(ifs, line) )
	{		
		std::stringstream ss{line};	
		ss >> value;
		return (int)value / 1000;
	}
	return 0;
}

uint64_t get_storage_info(){
	struct statvfs fiData;
	statvfs(".",&fiData);
	uint64_t free_space =  fiData.f_bfree * fiData.f_bsize;
	return free_space ;
	}

std::pair <uint64_t, uint64_t>  get_network_info(){
	constexpr const char* netinfo_file 		= "/proc/net/dev";
	
	auto ifs = std::ifstream{netinfo_file};
	if(!ifs.good()){
		throw std::runtime_error("Error: unable to memory-info file.");
	}
	std::string line, label;
	uint64_t received_sec, sent_sec, aux_r, aux_s;

	while( std::getline(ifs, line) ){		
		std::stringstream ss{line};	
		ss >> label;
		if(label == "wlp9s0:"){
			ss >> aux_r;
			for(int i = 1; i<=8; i++){
				ss >> aux_s;
			}
			//std::cout<<aux_r<<" "<<aux_s<<"\n";
			received_sec = aux_r - received;
			sent_sec = aux_s - sent ;
			received = aux_r;
			sent = aux_s;
			return std::make_pair(received_sec, sent_sec);

		}else{
			continue;
		}

	}

	return std::make_pair(0, 0);
	}

std::pair <int, int>  get_runtime_info(){

	auto ifs = std::ifstream("/proc/uptime");
	if( !ifs.good() ){ throw std::runtime_error("Error: unable to open uptime file "); }
	double seconds;
	ifs >> seconds;
	uint64_t factor1 = 60 * 60;
	uint64_t factor2 = 60;

	return std::make_pair ( seconds / factor1 , std::fmod(( seconds / factor2 ) , factor2) );
	}

};

class Chart {
	private:
	double values[121];
	std::string colors[5] = { "\x1B[31m", "\x1B[33m", "\x1B[93m", "\x1B[37m", "\x1B[36m" };
	int prev_rel_x, prev_rel_y, prev_rel_w, prev_rel_h;

	public:
	double new_value;
	int rel_x, rel_y;
	int rel_w, rel_h;

	Chart (){
		
		
		}
	void refresh_chart (){
		
		}
	void resize_chart (){
		
		}
	};

void gotoxy(short x, short y) {
#ifdef _WIN32
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    printf("%c[%d;%df", 0x1B, y, x);
#endif
}

int main (int argc, char **argv)
{
    	struct winsize w;
	constexpr int factor1 = 1024;
	constexpr int factor2 = 100;
	PC_info PC;

	while(1){
		//gotoxy(0,0);
		sleep(1);
	    	system("clear");

    		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
		printf ("lines %d\n", w.ws_row);
    		printf ("columns %d\n", w.ws_col);
		
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

		std::cout << "Uptime: " << x6.first << " hours  " << x6.second << " minutes" << "\n";
		std::cout << "RAM: " << x1.first << x1.second << "b used out of ";
		std::cout << x11.first << x11.second << "b \n";
		std::cout << "CPU Usage: "<< x2 << "% \n";
		std::cout << "CPU Temp: " << x3 << "°C \n";
		std::cout << "Main Filesystem: " << x4.first << x4.second << "b free of " << x45.first << x45.second  << "b \n";
		std::cout << "Network Data: " << x51.first << x51.second << "b/s received   " << x52.first << x52.second << "b/s sent" << "\n";

	}
    return 0;  // make sure your main returns int

}
