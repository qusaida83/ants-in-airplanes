// This file contains the setup definitions from the problem.

/*
 * setup parameters()
 *
 * This function setup the parameters needed to apply the ACO.
 * The parameters are:
 * 	number of ants
 * 	pheromone 
 */
void setup_parameters(){
	unsigned short int i,j;

	//setup for randomic numbers
	srand(time(NULL));

	//setup number of turns without improvement to end

	//setup max_pheromone
	unsigned long int max_late = 0;
	unsigned long int max_early = 0;
	unsigned long long int last_max_pheromone = 0; //used to check for overflow
	unsigned int delta_time = 0;
	for(i=0;i<planes_n;i++){
		last_max_pheromone = max_pheromone;

		delta_time = airplanes[i].latest_lt - airplanes[i].target_lt;
		max_late = delta_time * airplanes[i].cost_after;
		delta_time = airplanes[i].target_lt - airplanes[i].earliest_lt;
		max_early = delta_time * airplanes[i].cost_before;

		if(max_late > max_early)
			max_pheromone += max_late;
		else
			max_pheromone += max_early;

		if(last_max_pheromone > max_pheromone)
			critical_error("Overflow at defining max pheromone");
	}
	if((max_pheromone*max_pheromone) < max_pheromone)
		critical_error("max pheromone may overflow on the usage.");

	//setup default edge value
	edge_heuristic_value = (max_pheromone * 80)/100;
	
	//setup default edge value
	pheromone_evap_rate = (max_pheromone * 20)/100;

	//turns without improve is the end criteria 
	turns_without_improve_to_end = planes_n*2;

	best_global_solution = max_pheromone;
	//setup the pheromone matrix; It has no pheromones,
	//so only the heuristic value is used
	pheromone_matrix = (unsigned long long int **)malloc(sizeof(unsigned long long int *)*planes_n);
	for(i=0; i<planes_n; i++)
		pheromone_matrix[i] = (unsigned long long int *)malloc(sizeof(unsigned long long int)*planes_n);

	for(i=0; i<planes_n; i++)
		for(j=0; j<planes_n; j++){
			pheromone_matrix[i][j] = 0; //airplanes[i].target_lt - airplanes[j].target_lt;
		}

	heuristic_matrix = (unsigned long long int **)malloc(sizeof(unsigned long long int *)*planes_n);
	for(i=0; i<planes_n; i++)
		heuristic_matrix[i] = (unsigned long long int *)malloc(sizeof(unsigned long long int)*planes_n);
	for(i=0; i<planes_n; i++)
		for(j=0; j<planes_n; j++){
			heuristic_matrix[i][j] = edge_heuristic_value ;
		}
	//setup number of ants
	ants_n = planes_n*2;

	//setup starting plane
	starting_plane = rand() % planes_n ;

	//setup ants
	ants = (struct ant *)malloc(sizeof(struct ant)*ants_n);
	for(i=0; i<ants_n; i++){
		ants[i].solution = 0; //forces an overflow to be the lowest possible.
		ants[i].planes_lt= (long int *)malloc(sizeof(long int)*planes_n);
		ants[i].planes_path = (unsigned int *)malloc(sizeof(unsigned int)*planes_n);
		for(j=0; j<planes_n; j++){
			ants[i].planes_lt[j] = -1;
			ants[i].planes_path[j] = -1;
		}
	}

}

void print_setup(){
	puts("===SETUP DATA===");
	printf("max_pheromone = %llu\n", max_pheromone);
	printf("edge_heuristic_value = %lu\n", edge_heuristic_value);
	printf("ants_n = %d\n", ants_n);
	printf("starting_plane= %d\n", starting_plane);

	unsigned int i,j;
	puts("pheromone_matrix:");
	for(i = 0; i< planes_n ; i++){
		for(j = 0 ; j < planes_n ; j++){
			printf("%llu ",pheromone_matrix[i][j]);
		}
		puts("");
	}
	puts("");
	puts("ants:");
	for(i = 0; i< ants_n ; i++){
		printf("Ant %d:\n",i);
		printf("\tSolution = %llu\n",ants[i].solution);
		printf("\tplanes_lt:\n");
		for(j = 0 ; j < planes_n ; j++)
			printf("\t\tavião %d:%ld\n",j, ants[i].planes_lt[j]);
	}
	puts("");
}
