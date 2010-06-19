//This file contains some helper functions used in all libraries
//and the definition of globals.

unsigned long long int max_pheromone = 0;
unsigned long int edge_heuristic_value = 0;
unsigned long int ** pheromone_matrix;
unsigned short int ants_n = 0;
unsigned short int starting_plane = 0;
struct ant * ants;
struct airplane * airplanes;
int planes_n;
int ** separation_time;

critical_error(const char * e){
	fprintf(stderr, "Critical error: %s\n", e);
	exit(1);
}
