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
#include "aco.c"

int main(int argc, const char *argv[]){
	parser("instance/airland1.txt");
	//print_extracted_data();

	setup_parameters();
	//print_setup();

	do{
		generate_solutions();
		refresh_pheromone();
	}while(not_end());
	printf("Melhor solução: %llu\n",best_global_solution);
}
