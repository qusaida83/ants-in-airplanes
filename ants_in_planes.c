#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "parser.c"

/*
 * setup parameters()
 *
 * This function setup the parameters needed to apply the ACO.
 * The parameters are:
 * 	number of ants
 * 	pheromone 
 */


unsigned long long int max_pheromone = 0;
unsigned long int edge_heuristic_value = 0;
unsigned long int ** pheromone_matrix;
unsigned short int ants_n = 0;
unsigned short int starting_plane = 0;


critical_error(const char * e){
	fprintf(stderr, "Critical error: %s\n", e);
	exit(1);
}

void setup_parameters(){
	unsigned short int i,j;

	//setup for randomic numbers
	srand(time(NULL));

	//setup max_pheromone
	//TODO: review? 
	//update: works for airplan9.txt
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
	edge_heuristic_value = (max_pheromone * 90)/100;
	
	//setup the pheromone matrix; It has no pheromones,
	//so only the heuristic value is used
	pheromone_matrix = (unsigned long int **)malloc(sizeof(long int *)*planes_n);
	for(i=0; i<planes_n; i++)
		pheromone_matrix[i] = (long int *)malloc(sizeof(long int)*planes_n);

	for(i=0; i<planes_n; i++)
		for(j=0; j<planes_n; j++)
			pheromone_matrix[i][j] = edge_heuristic_value;

	//setup number of ants
	ants_n = planes_n/2;

	//setup starting plane
	starting_plane = rand() % planes_n ;

}

void print_setup(){
	puts("===SETUP DATA===");
	printf("max_pheromone = %d\n", max_pheromone);
	printf("edge_heuristic_value = %d\n", edge_heuristic_value);
	printf("ants_n = %d\n", ants_n);
	printf("starting_plane = %d\n", starting_plane);

	int i,j;
	puts("pheromone_matrix:");
	for(i = 0; i< planes_n ; i++){
		for(j = 0 ; j < planes_n ; j++){
			printf("%d ",pheromone_matrix[i][j]);
		}
		puts("");
	}
	puts("");
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
