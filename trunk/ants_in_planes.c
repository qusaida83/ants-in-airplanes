#include <stdio.h>
#include <stdlib.h>
#include "parser.c"

/*
 * setup parameters()
 *
 * This function setup the parameters needed to apply the ACO.
 * The parameters are:
 * 	number of ants
 * 	pheromone 
 */


long int max_pherormone = 0;

void setup_parameters(){
	int i;

	//setup max_pherormone
	long int max_late;
	long int max_early;
	for(i=0;i<planes_n;i++){
		max_late = airplanes[i].latest_lt * airplanes[i].cost_after;
		max_early = airplanes[i].earliest_lt * airplanes[i].cost_before;
		if(max_late > max_early)
			max_pherormone += max_late;
		else
			max_pherormone += max_early;
	}


}

void print_setup(){
	puts("===SETUP DATA===");
	printf("max_pherormone = %d\n", max_pherormone);
}






int main(int argc, const char *argv[]){
	parser("instance/airland1.txt");
	print_extracted_data();

	setup_parameters();
	print_setup();
/*
	while(not_end()){
		generate_solutions();
		refresh_pheromone();
	}*/
}
