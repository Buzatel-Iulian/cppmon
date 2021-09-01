//#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 
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

std::string readFile(std::string const& file)
{
    std::ifstream is(file);
    if( !is.good() ){
        throw std::runtime_error("Error: stream has errors.");
    }
    std::stringstream ss;
    ss << is.rdbuf();
	std::string m;
	// Remove ending line character '\n' or '\r\n'.
	std::getline(ss, m);
    return m;
}

std::string olc_grep (std::string const& file, std::string const& info){
		auto ifs = std::ifstream{file};
		if(!ifs.good()){
			throw std::runtime_error("Error: unable to memory-info file.");
		}
		std::string line, label; 
		while( std::getline(ifs, line) )
		{		
			std::stringstream ss{line};	
			while (ss >> label){
			//ss >> label >> value;
			if(label == info){
				return line ;
			}else{
				continue;
			}}
		}
	return "fie or label not found";
	}

class PC_info{

	private:

	public:
	double total_RAM;
	uint64_t total_space, received, sent;
        std::string Machine_Hostname, Kernel_Version, OS_type, Kernel_compile_date, CPU_name;  

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
		Machine_Hostname = readFile("/proc/sys/kernel/hostname");
		Kernel_Version = readFile("/proc/sys/kernel/osrelease");
		OS_type = readFile("/proc/sys/kernel/ostype");
		Kernel_compile_date = readFile("/proc/sys/kernel/version");
		CPU_name = "CPU " + olc_grep("/proc/cpuinfo","name"); 
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


