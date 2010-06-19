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

generate_solutions(){
	int i,j,k,l;
	int planes_visited = 0;
	int current_plane = starting_plane;
	printf("Current plane = %d\n",current_plane);

	int best_early_time = airplanes[current_plane].target_lt-1;
	int best_late_time = airplanes[current_plane].target_lt+1;
	int target_time = airplanes[current_plane].target_lt;
	int occuped_late_time = 0;
	int occuped_early_time = 0;
	int invalid_solution = 0;

	//be ready for the magic.
	//
	//Note: fotanus, tree years from now, 
	//don't try to understand; just rewrite.
	//
	//you were warned.
	for(i=0; i<ants_n; i++){ //for each ant
		while(planes_visited < planes_n){ //for each plane
			for(k=0; k<planes_n; k++){ // for each other plane ...
				if( ants[i].visited_planes[k] > 0){ // ... that was already visited

					//some alias to help 
					occuped_early_time = ants[i].visited_planes[k] - separation_time[current_plane][k];
					occuped_late_time = ants[i].visited_planes[k] + separation_time[current_plane][k];


					//case 1: the plane analized landed after the current target landing time
					if( airplanes[current_plane].target_lt > ants[i].visited_planes[k]){
						best_early_time = occuped_late_time;
						if( best_early_time != -1){

							if(occuped_early_time > best_early_time)
								best_early_time = occuped_early_time;

							if(best_early_time > airplanes[current_plane].target_lt){
								target_time = -1;
								best_early_time = -1;
							}
						}
						if(best_early_time == -1){
							if( best_early_time > best_late_time)
								best_late_time = best_early_time;
						}
					}

					//case 2: the plane analized landed before the current target landing time
					else if( airplanes[current_plane].target_lt < ants[i].visited_planes[k]){
						if( best_late_time != -1){

							if(occuped_late_time < best_late_time)
								best_late_time = occuped_early_time;

							if(best_late_time < airplanes[current_plane].target_lt){
								target_time = -1;
								best_late_time = -1;
							}
						}
						if(best_late_time == -1){
							if( best_late_time < best_early_time)
								best_early_time = best_late_time;
						}
					}
					//case 3: the plane analized landed EXACTLY on the current target landing time
					else{
						if(best_early_time != -1)
							best_early_time = occuped_early_time;
						if(best_late_time != -1)
							best_late_time = occuped_late_time;
						target_time = -1;
					}
				}


			}
			//decides what is the best solution for this plane
			//
			if(target_time != -1) //can use target time
				ants[i].visited_planes[current_plane] = target_time;
			else if(best_early_time != -1)
				if(best_late_time != -1) //need to choose between early and late
					if((target_time - best_early_time) * airplanes[current_plane].cost_before <
					   (best_late_time - target_time) * airplanes[current_plane].cost_after)
						ants[i].visited_planes[current_plane] = best_early_time;
					else 
						ants[i].visited_planes[current_plane] = best_late_time;
				else //can only pick early
					ants[i].visited_planes[current_plane] = best_early_time;
			else if(best_late_time != -1) //can only pick late
				ants[i].visited_planes[current_plane] = best_late_time;
			else{ //solution was invalid
				puts("Nooo! Invalid solution!!");
				invalid_solution = 1;
				break;
			}

			printf("Ant%d: For the plane %d I've choosed the time %d.\n",i,current_plane, ants[i].visited_planes[current_plane]);
			++planes_visited;

			int next_plane;
			if(planes_visited < planes_n){
				do{
					current_plane = rand() % planes_n ;
				} while(ants[i].visited_planes[current_plane] != 0);

				best_early_time = airplanes[current_plane].target_lt-1;
				best_late_time = airplanes[current_plane].target_lt+1;
				target_time = airplanes[current_plane].target_lt;
			}

		}

		if(invalid_solution == 0){
			//calculate the solution
			for(j=0; j<planes_n; j++){
				if(ants[i].visited_planes[j] < airplanes[j].target_lt)
					ants[i].solution += (airplanes[j].target_lt - ants[i].visited_planes[j]) * airplanes[j].cost_before;
				else if(ants[i].visited_planes[j] > airplanes[j].target_lt)
					ants[i].solution += (ants[i].visited_planes[j] - airplanes[j].target_lt ) * airplanes[j].cost_after;
			}
			printf("Ant%i: My solution was %d!\n",i,ants[i].solution);
		}

		if(invalid_solution == 1)
			i--; //same ant try again

		//setup environment for next ant
		planes_visited = 0;
		invalid_solution = 0;
		current_plane = starting_plane;
		best_early_time = airplanes[current_plane].target_lt-1;
		best_late_time = airplanes[current_plane].target_lt+1;
		target_time = airplanes[current_plane].target_lt;
	}

}
int main(int argc, const char *argv[]){
	parser("instance/airland9.txt");
	print_extracted_data();

	setup_parameters();
	print_setup();

	generate_solutions();
	puts("end");
/*
	while(not_end()){
		generate_solutions();
		refresh_pheromone();
	}
*/
}
