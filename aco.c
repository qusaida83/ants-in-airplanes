
int not_end(){
	unsigned short int i;
	unsigned long long int  max_solution_round = max_pheromone;

	//define best solution on round
	for(i=0; i<ants_n; i++)
		if(ants[i].solution < max_solution_round){
			max_solution_round = ants[i].solution;
/*			if(max_solution_round < 700){
				ant_talks(i);
				exit(1);
			}*/
		}

	//check if had improvement
	if(max_solution_round < best_global_solution){
		best_global_solution = max_solution_round;
		printf("solução parcial: %llu\n",best_global_solution);
		turns_without_improve = 0;
	}
	else
		++turns_without_improve;

	if(turns_without_improve < turns_without_improve_to_end)
		return 1;
	else
		return 0;

}


void generate_solutions(){
	int i;

	for(i=0; i<ants_n; i++){ //for each ant

		//setup environment for next ant
		initialize_ant(i);
		setup_env();

		while(planes_visited < planes_n){ //for each plane
			//printf("Current plane: %d\n",current_plane);

			ants[i].planes_path[planes_visited] = current_plane; //keeps the trace of visited planes.
			++planes_visited;

			init_range_list();
			write_ranges(i);

			init_possible_times();
			fill_possible_times();

			free(range.ranges);

			decide_best_land_time(i);

			if(planes_visited < planes_n){
				choose_next_plane(i);
			}

			free(possible_times);

		}// close visited planes
		//ant_talks(i);
		if (impossible_solution == 1){
			puts("returning to ant");
			ants[i].solution = max_pheromone;
			//i--; //return one ant
		}
	}//close ants
}


