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


unsigned long int max_pherormone = 0;
unsigned long int arch_heuristic_value = 0;
unsigned long int ** pherormone_matrix;
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

	//setup max_pherormone
	unsigned long int max_late = 0;
	unsigned long int max_early = 0;
	unsigned long int last_max_pherormone = 0; //used to check for overflow
	unsigned int delta_time = 0;
	for(i=0;i<planes_n;i++){
		last_max_pherormone = max_pherormone;
		delta_time = airplanes[i].latest_lt - airplanes[i].target_lt;
		max_late = delta_time * airplanes[i].cost_after;
		max_early = delta_time * airplanes[i].cost_before;
		if(max_late > max_early)
			max_pherormone += max_late;
		else
			max_pherormone += max_early;

		if(last_max_pherormone > max_pherormone)
			critical_error("Overflow at defining max pherormone");
	}
	if((2*max_pherormone) < max_pherormone)
		critical_error("max pherormone may overflow on the usage.");

	//setup default arch value
	arch_heuristic_value = (max_pherormone * 90)/100;
	
	//setup the pherormone matrix; It has no pherormones,
	//so only the heuristic value is used
	pherormone_matrix = (unsigned long int **)malloc(sizeof(long int *)*planes_n);
	for(i=0; i<planes_n; i++)
		pherormone_matrix[i] = (long int *)malloc(sizeof(long int)*planes_n);

	for(i=0; i<planes_n; i++)
		for(j=0; j<planes_n; j++)
			pherormone_matrix[i][j] = arch_heuristic_value;

	//setup number of ants
	ants_n = planes_n/2;

	//setup starting plane
	starting_plane = rand()%planes_n ;

}

void print_setup(){
	puts("===SETUP DATA===");
	printf("max_pherormone = %d\n", max_pherormone);
	printf("arch_heuristic_value = %d\n", arch_heuristic_value);
	printf("ants_n = %d\n", ants_n);
	printf("starting_plane = %d\n", starting_plane);

	int i,j;
	puts("pherormone_matrix:");
	for(i = 0; i< planes_n ; i++){
		for(j = 0 ; j < planes_n ; j++){
			printf("%.5d ",pherormone_matrix[i][j]);
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
