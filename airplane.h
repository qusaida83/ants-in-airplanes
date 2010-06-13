
//appearance time, earliest landing time, target landing time, latest landing time, penalty cost per unit of time for landing before target, penalty cost per unit of time for landing after target
struct airplane{
	int appearance;
	int earliest_lt;
	int target_lt;
	int latest_lt;
	float cost_before;
	float cost_after;
};

struct airplane * airplanes;
int planes_n;
int ** separation_time;
