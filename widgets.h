#include "ui_methods.h"

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
