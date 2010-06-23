#include <stdio.h>

/*
 * funcao parse
 * essa funcao parseia o arquivo do problema.
 */
void parser(char * input_file_path){

	puts("Inicializando o arquivo...");
	FILE * input_file = fopen(input_file_path, "r");

	puts("Lendo arquivo....");
	float x;
	int i;

	//Firstly we read the number of airplanes and use it to
	//create basic structure (airplanes and separation_time)
	fscanf(input_file, "%f", &x);
	planes_n = x;

	airplanes = (struct airplane *)malloc(sizeof(struct airplane)*planes_n);

	separation_time = (int **)malloc( sizeof(int *)*planes_n);
	for(i=0;i<planes_n;i++){
			separation_time[i] = (int *)malloc(sizeof(int)*planes_n);
	}

	//just ignore the freeze time; we don't need it.
	fscanf(input_file, "%f", &x);


	//this block grab data from file and puts in planes structure.
	//this also fills the separation_time matrix.
	int current_plane;
	for(current_plane = 0; current_plane<planes_n; current_plane++){
		//grab fix data for the plane
		fscanf(input_file, "%f", &x);
		airplanes[current_plane].appearance = x;
		fscanf(input_file, "%f", &x);
		airplanes[current_plane].earliest_lt = x;
		fscanf(input_file, "%f", &x);
		airplanes[current_plane].target_lt= x;
		fscanf(input_file, "%f", &x);
		airplanes[current_plane].latest_lt= x;
		fscanf(input_file, "%f", &x);
		airplanes[current_plane].cost_before= x;
		fscanf(input_file, "%f", &x);
		airplanes[current_plane].cost_after= x;

		//fill the first line of the matrix.
		int i;
		for(i=0; i<planes_n; i++){
			fscanf(input_file, "%d",&separation_time[current_plane][i]);
		}
	}

        fclose(input_file);
}



//This function is used for debug porpose only.
//It prints the data retrived from planes.

void print_extracted_data(){
	int i,j;
	for(i = 0; i< planes_n; i++){
		printf("\n\n=====Aviao %d=====\n",i);
		printf("%d\n",airplanes[i].appearance);
		printf("%d\n",airplanes[i].earliest_lt);
		printf("%d\n",airplanes[i].target_lt);
		printf("%d\n",airplanes[i].latest_lt);
		printf("%f\n",airplanes[i].cost_before);
		printf("%f\n",airplanes[i].cost_after);
	}

	puts("");
	puts("====Matriz====");
	for(i = 0; i< planes_n ; i++){
		for(j = 0 ; j < planes_n ; j++){
			printf("%.5d ",separation_time[i][j]);
		}
		puts("");
	}
	puts("");
}
