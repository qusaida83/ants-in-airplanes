
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


generate_solutions(){
	int i,j,k;

	for(i=0; i<ants_n; i++){ //for each ant

		//setup environment for next ant
		initialize_ant(i);
		setup_env();
		while(planes_visited < planes_n){ //for each plane

			ants[i].planes_path[planes_visited] = current_plane; //keeps the trace of visited planes.
			init_range_list();
			write_ranges(i);

			init_possible_times();
			fill_possible_times();

			free(range.ranges);
			range.last_read_pos = 0;

			decide_best_land_time(i);

			++planes_visited;

			if(planes_visited < planes_n){
				choose_next_plane(i);
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


void choose_next_plane(int i){
	int j;

	current_plane= rand() % planes_n ;
	
	unsigned long long int prob_sum=0;
	int entrou = 0;
	unsigned long long int ultima = 0;
	for(j=0; j<planes_n; j++){
		if(ants[i].planes_lt[j] == 0){
			//printf("planes visited = %d\n",ants[i].planes_path[planes_visited]);
			//printf("planes visited-1 = %d\n",ants[i].planes_path[planes_visited-1]);
			prob_sum += (pheromone_matrix[ants[i].planes_path[planes_visited-1]][j]);
			ultima =  (pheromone_matrix[ants[i].planes_path[planes_visited-1]][j]);
		}
	}
	if(prob_sum == 0){
		//VERIFY BREAK!!
		impossible_solution = 1; // just belive it
		
		printf("Entrou %d vezes\n",entrou);
		if(entrou_uma_vez==1){
			puts("Ja foi impossivel");
		}
		printf("ultima: %llu \n",ultima);
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
				//DO SOMETHING!!!
			}
		}
	}
}
