#include "ui_methods.h"
#include <vector>
#include <iostream>

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
	void display (){
		
		}
	void hide (){
		
		}
	void update (){

		}
	};

class Menu {
	private:
	
	public:
	Menu (){}
	void up_opt(){}
	void dwn_opt(){}
	void display(){}
	void hide(){}

	};

class Label {
	public:
	std::vector <std::string> content ;
	short x,y;
	std::vector <std::string> h = {H_BLK,H_DEFAULT};

	Label ( std::string text , short x = 0, short y = 0){
		this->x = x;
		this->y = y;

		int start = 0;
		std::string del = "\n";
		int end = text.find(del);
		while (end != -1) {
        
			content.push_back(text.substr(start, end - start));
        		start = end + del.size();
        		end = text.find(del, start);
		}
		content.push_back(text.substr(start, end - start));

	}
	
	void display(){
		short x1=1, x2=0, x3=0;
		for (int i=0;i<content.size();i++ ){
			gotoxy(x,y+i);
			std::cout << s_format( content[i], CYN, h[x1] ) ;
			x3=x1; x1=x2; x2=x3;
			}
		gotoxy(0,0);
		}
	void hide(){}
	};
