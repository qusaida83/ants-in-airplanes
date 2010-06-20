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
	ants[ant_n].solution = max_pheromone;
	for(i=0; i<planes_n; i++){
		ants[ant_n].planes_path[i]=0;
		ants[ant_n].planes_lt[i]=0;
	}

}

void ant_talks(int i){
	int j;
	printf("Ant %d reports:\n",i);
	printf("Ant%d: My solution was %d!\n",i,ants[i].solution);
	printf("Ant%d: I did the following path:\n",i);
	for(j=0; j<planes_n; j++)
		printf("\tPlane %d\n",ants[i].planes_path[j]);
	printf("Ant %d salutes and was dispensed\n\n");
}


generate_solutions(){
	puts("====NEW ROUND====");
	int i,j,k,l;

	int occuped_late_time = 0; //this two variables must always be set before used. They are
	int occuped_early_time = 0;//just alias to help readability. 

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
		unsigned char invalid_solution = 0;
		unsigned short int current_plane = starting_plane;
		unsigned int best_early_time = airplanes[current_plane].target_lt-1;
		unsigned int best_late_time = airplanes[current_plane].target_lt+1;
		unsigned int target_time = airplanes[current_plane].target_lt;

		while(planes_visited < planes_n){ //for each plane

			ants[i].planes_path[planes_visited] = current_plane; //keeps the trace of visited planes.

			for(k=0; k<planes_n; k++){ // for each other plane ...
				if( ants[i].planes_lt[k] > 0){ // ... that was already visited

					//some alias to help
					//delimites the range which the plane can't land.
					occuped_early_time = ants[i].planes_lt[k] - separation_time[current_plane][k];
					occuped_late_time = ants[i].planes_lt[k] + separation_time[current_plane][k];

					//case 1: the plane analized landed before the current target landing time
					if( airplanes[current_plane].target_lt > ants[i].planes_lt[k]){
						if( best_early_time != -1){
							if(occuped_late_time > best_early_time)
								best_early_time = occuped_late_time;

							//this checks for overflow; i.e. when the analized plane landing time
							//get to or trespass the current plane target landing time.
							if(best_early_time == airplanes[current_plane].target_lt)
								best_early_time = -1;
							else if(best_early_time > airplanes[current_plane].target_lt){
								target_time = -1;
								best_early_time = -1;
							}
						}
						if(best_early_time == -1){
							if( occuped_late_time > best_late_time)
								best_late_time = occuped_late_time;
						}
					}

					//case 2: the plane analized landed after the current target landing time
					else if( airplanes[current_plane].target_lt < ants[i].planes_lt[k]){
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
				} // end if was visited
			} // end to analize the conflict time with this plane.

			//at this point, for the current_plane, all other planes that
			//had the landing time already decided were checked.

			//decides what is the best solution for this plane
			if(target_time != -1) //can use target time
				ants[i].planes_lt[current_plane] = target_time;
			else if(best_early_time != -1)
				if(best_late_time != -1) //need to choose between early and late
					if((target_time - best_early_time) * airplanes[current_plane].cost_before <
					   (best_late_time - target_time) * airplanes[current_plane].cost_after)
						ants[i].planes_lt[current_plane] = best_early_time;
					else 
						ants[i].planes_lt[current_plane] = best_late_time;
				else //can only pick early
					ants[i].planes_lt[current_plane] = best_early_time;
			else if(best_late_time != -1) //can only pick late
				ants[i].planes_lt[current_plane] = best_late_time;
			else{ //solution was invalid
				printf("Ant%d: Nooo! Invalid solution! I want to be dispensed!\n",i);
				invalid_solution = 1;
				break;
			}

			//printf("Ant%d: For the plane %d I've choosed the time %d.\n",i,current_plane, ants[i].planes_lt[current_plane]);
			++planes_visited;

			int next_plane;
			if(planes_visited < planes_n){
				do{
					//TODO: Add ferormonies and heuristical 
					current_plane = rand() % planes_n ;
				} while(ants[i].planes_lt[current_plane] != 0);

				best_early_time = airplanes[current_plane].target_lt-1;
				best_late_time = airplanes[current_plane].target_lt+1;
				target_time = airplanes[current_plane].target_lt;
			}

		}

		//if the solutions is valid, calculate stuff
		if(invalid_solution == 0){
			for(j=0; j<planes_n; j++){
				if(ants[i].planes_lt[j] < airplanes[j].target_lt)
					ants[i].solution += (airplanes[j].target_lt - ants[i].planes_lt[j]) * airplanes[j].cost_before;
				else if(ants[i].planes_lt[j] > airplanes[j].target_lt)
					ants[i].solution += (ants[i].planes_lt[j] - airplanes[j].target_lt ) * airplanes[j].cost_after;
				//else is the target landing time and don't chaes the solution
			}
			ant_talks(i);
		}
		else
			--i; //same ant try again

	}

}

void refresh_pheromone(){
	int i,j;
	int pheromone = 0;

	//puts the new pheromone
	for(i=0; i<ants_n; i++){
		pheromone = max_pheromone - ants[i].solution;
		for(j=1; j<planes_n; j++){
			pheromone_matrix[j-1][j] += pheromone;
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
		if(ants[i].solution < max_solution_round)
			max_solution_round = ants[i].solution;

	//check if had improvement
	if(max_solution_round < best_global_solution){
		puts("CHANGES");
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
	parser("instance/airland1.txt");
	//print_extracted_data();

	setup_parameters();
	//print_setup();

	while(not_end()){
		generate_solutions();
		refresh_pheromone();
	}
	printf("Melhor solução: %d\n",best_global_solution);
}
