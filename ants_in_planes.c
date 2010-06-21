#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "common.c"
#include "ant.h"
#include "airplane.h"
#include "parser.c"
#include "setup.c"


/*
 * arguably the main function of the program. It describes how
 * ants walk through the graph. 
 */

void initialize_ant(int ant_n){
	int i;
	ants[ant_n].solution = 0;
	for(i=0; i<planes_n; i++){
		ants[ant_n].planes_path[i]=0;
		ants[ant_n].planes_lt[i]=0;
	}
}

void ant_talks(int i){
	int j;
	printf("Ant %d reports:\n",i);
	printf("Ant%d: My solution was %llu!\n",i,ants[i].solution);
	printf("Ant%d: I did the following path:\n",i);
	for(j=0; j<planes_n; j++){
		printf("\tPlane %d",ants[i].planes_path[j]);
		if( ants[i].planes_lt[ants[i].planes_path[j]] == airplanes[ants[i].planes_path[j]].target_lt)
			printf(" - And it was in target time %d!",airplanes[ants[i].planes_path[j]].target_lt);
		else
			printf(" - Landed at time %d!",ants[i].planes_lt[ants[i].planes_path[j]]);
		puts("");
	}
	printf("Ant %d salutes and was dispensed\n\n", i);
}

struct ranges_list{
	int num_elem;
	int last_written_pos;
	int last_read_pos;
	int * ranges;
};

init_range_list(struct ranges_list * r){
	r->num_elem = 0;
	r->last_written_pos = 0;
	r->last_read_pos = 0;
	r->ranges = (int *)malloc(sizeof(int)*planes_n*2);
}


generate_solutions(){
	puts("====NEW ROUND====");
	int i,j,k;

	//be ready for the magic.
	//
	//Note: fotanus, tree years from now, 
	//don't try to understand; just rewrite.
	//
	//you were warned.
	for(i=0; i<ants_n; i++){ //for each ant

		//setup environment for next ant
		initialize_ant(i);
		unsigned short int planes_visited = 0;
		unsigned short int current_plane = starting_plane;
		char impossible_solution = 0;

		while(planes_visited < planes_n){ //for each plane
			struct ranges_list range;
			init_range_list(&range);

			ants[i].planes_path[planes_visited] = current_plane; //keeps the trace of visited planes.

			//gets all ranges from already landed planes.
			for(j=0; j<planes_n; j++){ // for each other plane ...
				if( ants[i].planes_lt[j] > 0){ // ... that was already visited

					//some alias to help
					int min_range = ants[i].planes_lt[j] - separation_time[current_plane][j];
					int max_range = ants[i].planes_lt[j] + separation_time[current_plane][j];
					range.ranges[range.last_written_pos] = min_range;
					range.ranges[range.last_written_pos+1] = max_range;
					range.last_written_pos += 2;
					range.num_elem++;
				} // end to analize the conflict time with this plane.
			}

			/*
			puts("RANGE:");
			for(j=0; j<range.num_elem; j++){
				printf("%d - %d\n" ,range.ranges[range.last_read_pos], range.ranges[range.last_read_pos+1]);
				range.last_read_pos +=2;
			}
			range.last_read_pos =0;
			*/
			// at this point, we have all ranges that we can't land this plane.
			// | ---- N ----- |      //the plane we must decide the landing time
			//   |-1-|     |--2---|  //the already landed planes ranges that restrict our landing time
			// |-|   |-----|         //possible landing range.

			// initialize a vector that describes the possible landing times
			int possible_times_size = airplanes[current_plane].latest_lt - airplanes[current_plane].earliest_lt + 1;
			char * possible_times = (char *)malloc(sizeof(char) * possible_times_size );
			for (j = 0; j < possible_times_size; j++) {
				possible_times[j] = '0';
			}
			
			//for each range, fills the impossible times
			for(j=0; j<range.num_elem; j++){
				int for_init;
				int for_end;

				if( range.ranges[range.last_read_pos+1] < airplanes[current_plane].earliest_lt) {
					for_init=0;
					for_end = -1;
					//out of scope
				}else if(range.ranges[range.last_read_pos] > airplanes[current_plane].latest_lt){
					for_init=0;
					for_end = -1;
					//out of scope
				}else{
					if(range.ranges[range.last_read_pos] > airplanes[current_plane].earliest_lt){
						for_init = range.ranges[range.last_read_pos] - airplanes[current_plane].earliest_lt;
					}else{
						for_init = 0;
					}
					if(range.ranges[range.last_read_pos+1] < airplanes[current_plane].latest_lt){
						for_end = range.ranges[range.last_read_pos+1] - airplanes[current_plane].earliest_lt;
					}else{
						for_end = possible_times_size;
					}
				}
				/*
				printf("Target time do current: %d\n", airplanes[current_plane].target_lt);
				printf("Range: %d --- %d\n", range.ranges[range.last_read_pos], range.ranges[range.last_read_pos+1]);
				printf("For range: %d --- %d\n", for_init, for_end);
				*/
				//printf("For init: %d   | for end: %d\n",for_init,for_end);
				for(k=for_init; k<=for_end; k++)
					possible_times[k]='1';
				range.last_read_pos+=2;

			}
			free(range.ranges);
			range.last_read_pos = 0;

			/*
			puts("Bitmap:");
			for (j = 0; j < possible_times_size; j++) {
				printf("%c",possible_times[j]);
			}
			puts("");
			*/

			//decide best time, setting both the land time for this plane
			//and add in the solution value
			int target_pos = airplanes[current_plane].target_lt - airplanes[current_plane].earliest_lt;
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
				if( (late_pos == -1) && (early_pos == -1)){
					impossible_solution = 1;
				}
				
				else if((late_pos != -1) && (early_pos == -1)){
					ants[i].planes_lt[current_plane] = late_pos + airplanes[current_plane].earliest_lt;
					ants[i].solution += ( ants[i].planes_lt[current_plane] - airplanes[current_plane].target_lt ) * airplanes[current_plane].cost_after;

				}else if((late_pos == -1) && (early_pos != -1)){
					ants[i].planes_lt[current_plane] = early_pos + airplanes[current_plane].earliest_lt;
					ants[i].solution += ( airplanes[current_plane].target_lt - ants[i].planes_lt[current_plane]) * airplanes[current_plane].cost_before;
				}else{
					int late_real_pos = late_pos + airplanes[current_plane].earliest_lt;
					int early_real_pos = early_pos + airplanes[current_plane].earliest_lt;

					if(late_real_pos < early_real_pos){
						ants[i].planes_lt[current_plane] = late_pos + airplanes[current_plane].earliest_lt;
						ants[i].solution += ( ants[i].planes_lt[current_plane] - airplanes[current_plane].target_lt ) * airplanes[current_plane].cost_after;
					}else{
						ants[i].planes_lt[current_plane] = early_pos + airplanes[current_plane].earliest_lt;
						ants[i].solution += ( airplanes[current_plane].target_lt - ants[i].planes_lt[current_plane]) * airplanes[current_plane].cost_before;
					}
				}

			}

			++planes_visited;
			if(planes_visited < planes_n){
				unsigned long long int prob_sum=0;
				for(j=0; j<planes_n; j++){
					if(ants[i].planes_lt[j] == 0){
						//printf("planes visited = %d\n",ants[i].planes_path[planes_visited]);
						//printf("planes visited-1 = %d\n",ants[i].planes_path[planes_visited-1]);
						prob_sum += (pheromone_matrix[ants[i].planes_path[planes_visited-1]][j]);
					}
				}
				if(prob_sum == 0){
					printf("Max pheromone = %llu\n",max_pheromone);
					printf("Planes visited = %d\n",planes_visited);
					ant_talks(i);
					exit(1);
				}
				long long int ticket =  rand() % prob_sum;
				for(j=0; j<planes_n; j++){
					if(ants[i].planes_lt[j] == 0){
						ticket -= (pheromone_matrix[ants[i].planes_path[planes_visited-1]][j]);
						if (ticket < 0){
							current_plane = j;
							break;
						}
					}
				}
			}

			free(possible_times);

		}// close visited planes
		//ant_talks(i);
		if (impossible_solution == 1){
			puts("returning to ant");
			i--; //return one ant
		}
	}//close ants
}

void refresh_pheromone(){
	int i,j;
	int pheromone = 0;

	//puts the new pheromone
	for(i=0; i<ants_n; i++){
		pheromone = max_pheromone - (ants[i].solution);
		for(j=1; j<planes_n; j++){
			pheromone_matrix[ants[i].planes_path[j-1]][ants[i].planes_path[j]] += pheromone;
		}
	}

	//evaporates the pheromone
	for(i=0; i<planes_n; i++)
		for(j=0; j<planes_n; j++)
			pheromone_matrix[i][j] -= pheromone_evap_rate;
}



int not_end(){
	unsigned short int i,j;
	unsigned long long int  max_solution_round = max_pheromone;

	//define best solution on round
	for(i=0; i<ants_n; i++)
		if(ants[i].solution < max_solution_round){
			if(ants[i].solution >= 0){
				max_solution_round = ants[i].solution;
/*				if(max_solution_round < 700){
					ant_talks(i);
					exit(1);
				}*/
			}
		}

	//check if had improvement
	if(max_solution_round < best_global_solution){
		best_global_solution = max_solution_round;
		turns_without_improve = 0;
	}
	else
		++turns_without_improve;

	if(turns_without_improve < turns_without_improve_to_end)
		return 1;
	else
		return 0;

}


int main(int argc, const char *argv[]){
	parser("instance/airland8.txt");
	print_extracted_data();

	setup_parameters();
	//print_setup();

	do{
		generate_solutions();
		refresh_pheromone();
		printf("solução parcial: %llu\n",best_global_solution);
	}while(not_end());
	printf("Melhor solução: %llu\n",best_global_solution);

}
