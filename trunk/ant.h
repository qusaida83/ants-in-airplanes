struct ant{
	long int * planes_lt;
	unsigned int * planes_path;
	unsigned long long int solution;
};

void initialize_ant(int ant_n){
	unsigned int i;
	ants[ant_n].solution = 0;
	for(i=0; i<planes_n; i++){
		ants[ant_n].planes_path[i]=-1;
		ants[ant_n].planes_lt[i]=-1;
	}
}

//this should not be here, but i'm not sure where to put it.
void setup_env(){
	planes_visited = 0;
	current_plane = starting_plane;
	impossible_solution = 0;
}

void ant_talks(int i){
	unsigned int j;
	printf("Ant %d reports:\n",i);
	printf("Ant%d: My solution was %lld!\n",i,ants[i].solution);
	printf("Ant%d: I did the following path:\n",i);
	for(j=0; j<planes_n; j++){
		printf("\tPlane %d",ants[i].planes_path[j]);
		if( ants[i].planes_lt[ants[i].planes_path[j]] == airplanes[ants[i].planes_path[j]].target_lt)
			printf(" - And it was in target time %ld",airplanes[ants[i].planes_path[j]].target_lt);
		else
			printf(" - Landed at time %ld",ants[i].planes_lt[ants[i].planes_path[j]]);
		puts("");
	}
	printf("Ant %d salutes and was dispensed\n\n", i);
}

//should this be here?
void decide_best_land_time(int i){
	//decide best time, setting both the land time for this plane
	//and add in the solution value
	int target_pos = airplanes[current_plane].target_lt - airplanes[current_plane].earliest_lt;
	//printf("Target pos: %d\n",target_pos);
	if(possible_times[target_pos] == '0'){
		//if possible, gets the target landing time.
		ants[i].planes_lt[current_plane] = airplanes[current_plane].target_lt;
	}
	else{
		char find=0;
		int late_pos;
		int early_pos;
		int iterator = target_pos;

		//look early
		while( find == 0){
			--iterator;
			if(iterator >= 0){
				if(possible_times[iterator] == '0'){
					find = 1;
					early_pos = iterator;
				}
			}
			else{
				early_pos = -1;
				break;
			}
		}

		iterator = target_pos;
		find = 0;
		//look late
		while( find == 0){
			++iterator;
			if(iterator < possible_times_size){
				if(possible_times[iterator] == '0'){
					find = 1;
					late_pos = iterator;
				}
			}
			else{
				late_pos = -1;
				break;
			}
		}

		//printf("Early = %d, Late = %d\n",early_pos,late_pos);
		//printf("Solução da formiga %d: %d\n",i,ants[i].solution);
		//impossible
		if( (late_pos == -1) && (early_pos == -1) ){
			impossible_solution = 1;
			int k;
			for(k=0;k<planes_n;k++){
				ants[i].planes_lt[current_plane] = -1;
				ants[i].planes_path[planes_visited] = 0;
			}
			ants[i].solution = max_pheromone;
		}
		
		else if((late_pos != -1) && (early_pos == -1)){
			ants[i].planes_lt[current_plane] = late_pos + airplanes[current_plane].earliest_lt;
			ants[i].solution += ( ants[i].planes_lt[current_plane] - airplanes[current_plane].target_lt ) * airplanes[current_plane].cost_after;

		}else if((late_pos == -1) && (early_pos != -1)){
			ants[i].planes_lt[current_plane] = early_pos + airplanes[current_plane].earliest_lt;
			ants[i].solution += ( airplanes[current_plane].target_lt - ants[i].planes_lt[current_plane]) * airplanes[current_plane].cost_before;
		}else{
			int late_pos_cost = ((late_pos + airplanes[current_plane].earliest_lt)-airplanes[current_plane].target_lt)*airplanes[current_plane].cost_after;
			int early_pos_cost = (airplanes[current_plane].target_lt-(early_pos + airplanes[current_plane].earliest_lt))*airplanes[current_plane].cost_before;

			if(late_pos_cost < early_pos_cost){
				ants[i].planes_lt[current_plane] = late_pos + airplanes[current_plane].earliest_lt;
				ants[i].solution += late_pos_cost;
			}else{
				ants[i].planes_lt[current_plane] = early_pos + airplanes[current_plane].earliest_lt;
				ants[i].solution += early_pos_cost;
			}
		}
	}
}
