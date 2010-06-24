#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "airplane.h"
#include "common.c"
#include "ant.h"
#include "parser.c"
#include "setup.c"
#include "ranges.c"
#include "possible_times.c"
#include "pheromone.c"
#include "aco.c"

int main(){
	parser("instance/airland5.txt");
	//print_extracted_data();

	setup_parameters();
	//print_setup();
	do{
		generate_solutions();
		refresh_pheromone();
	}while(not_end());

	printf("Melhor solução: %llu\n",best_global_solution);
	return 0;
}
