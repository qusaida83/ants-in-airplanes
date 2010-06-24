
void refresh_pheromone(){
	unsigned int i,j;
	unsigned long long int pheromone = 0;

	//puts the new pheromone
	for(i=0; i<ants_n; i++){
		pheromone = max_pheromone - (ants[i].solution);
		if(ants[i].solution > max_pheromone)
			pheromone=0;

		for(j=1; j<planes_n; j++){
			unsigned long long int  overf = pheromone_matrix[ants[i].planes_path[j-1]][ants[i].planes_path[j]];
			pheromone_matrix[ants[i].planes_path[j-1]][ants[i].planes_path[j]] += pheromone;

			if( overf > pheromone_matrix[ants[i].planes_path[j-1]][ants[i].planes_path[j]] ){
				printf("Adding %llu\n",pheromone);
				printf("max %llu\n",max_pheromone);
				printf("solution %llu\n",ants[i].solution);
				printf("was %llu now is %llu\n",overf,pheromone_matrix[ants[i].planes_path[j-1]][ants[i].planes_path[j]]);
				critical_error("Overflow at adding pheromone");
			}

		}
	}

	//evaporates the pheromone
	for(i=0; i<planes_n; i++)
		for(j=0; j<planes_n; j++){
			unsigned long long int  overf = pheromone_matrix[i][j];
			pheromone_matrix[i][j] -= pheromone_evap_rate;
			if(pheromone_matrix[i][j] > overf){
				pheromone_matrix[i][j]=0;
			}
		}
}

void choose_next_plane(int i){
	/*
	do{
		current_plane= rand() % planes_n ;
	}while(ants[i].planes_lt[current_plane] != -1);
	*/
	unsigned int j;
	unsigned long long int prob_sum=0;
	int entrou = 0;
	unsigned long long int ultima = 0;
	for(j=0; j<planes_n; j++){
		if(ants[i].planes_lt[j] == -1){
			entrou++;
			prob_sum += (pheromone_matrix[ants[i].planes_path[planes_visited-1]][j]) + (heuristic_matrix[ants[i].planes_path[planes_visited-1]][j]) ;
		}
	}
	if(prob_sum == 0){
		impossible_solution = 1; // just belive it
		printf("Entrou %d vezes\n",entrou);
		printf("ultima: %llu \n",ultima);
		printf("Max pheromone = %llu\n",max_pheromone);
		printf("Planes visited = %d\n",planes_visited);
		ant_talks(i);
		critical_error("prob sum = 0");
	}
	long long int ticket =  rand() % prob_sum;
	for(j=0; j<planes_n; j++){
		if(ants[i].planes_lt[j] == -1){
			ticket -= (pheromone_matrix[ants[i].planes_path[planes_visited-1]][j])/100+(heuristic_matrix[ants[i].planes_path[planes_visited-1]][j]) ;
			if (ticket < 0){
				current_plane = j;
				break;
			}
		}
	}

}

