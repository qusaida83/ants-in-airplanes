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
	unsigned int i;

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
	unsigned int j;
	for(i= 0; i<planes_n; i++){
		//grab fix data for the plane
		fscanf(input_file, "%f", &x);
		airplanes[i].appearance = x;
		fscanf(input_file, "%f", &x);
		airplanes[i].earliest_lt = x;
		fscanf(input_file, "%f", &x);
		airplanes[i].target_lt= x;
		fscanf(input_file, "%f", &x);
		airplanes[i].latest_lt= x;
		fscanf(input_file, "%f", &x);
		airplanes[i].cost_before= x;
		fscanf(input_file, "%f", &x);
		airplanes[i].cost_after= x;

		//fill the first line of the matrix.
		for(j=0; j<planes_n; j++){
			fscanf(input_file, "%d",&separation_time[i][j]);
		}
	}

        fclose(input_file);
}



//This function is used for debug porpose only.
//It prints the data retrived from planes.

void print_extracted_data(){
	unsigned int i,j;
	for(i = 0; i< planes_n; i++){
		printf("\n\n=====Aviao %d=====\n",i);
		printf("%d\n",airplanes[i].appearance);
		printf("%ld\n",airplanes[i].earliest_lt);
		printf("%ld\n",airplanes[i].target_lt);
		printf("%ld\n",airplanes[i].latest_lt);
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
