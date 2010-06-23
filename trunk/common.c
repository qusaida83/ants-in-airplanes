//This file contains some helper functions used in all libraries
//and the definition of globals.

unsigned long long int max_pheromone = 0;
unsigned long long int pheromone_evap_rate = 0;
unsigned long int edge_heuristic_value = 0;
unsigned long int ** pheromone_matrix;
unsigned short int ants_n = 0;
unsigned short int starting_plane = 0;
unsigned short int turns_without_improve_to_end = 0;
unsigned short int turns_without_improve = 0;
unsigned long long int best_global_solution = 0;
struct ant * ants;
struct airplane * airplanes;
unsigned int planes_n;
int ** separation_time;
int entrou_uma_vez = 0;
int possible_times_size;
char * possible_times;
struct ranges_list range;

unsigned short int planes_visited = 0;
unsigned int current_plane;
char impossible_solution = 0;

void critical_error(const char * e){
	fprintf(stderr, "Critical error: %s\n", e);
	exit(1);
}
