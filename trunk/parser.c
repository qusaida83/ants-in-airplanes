#include <stdio.h>
#include "airplane.h"

/*
 * funcao parse
 * essa funcao parseia o arquivo do problema.
 */
void parser(char * input_file_path){

	enum state_t {INIT, };

        puts("Inicializando o arquivo...");
        FILE * input_file = fopen(input_file_path, "r");

        puts("Lendo arquivo....");
	float x=123;

	state_t state = INIT;
	while(fscanf(input_file, "%f", &x) != EOF){
		switch(state){

			case INIT:
				int planes_n = x;

				struct airplane * airplanes;
				airplane = (struct airplane *)malloc(sizeof(struct airplane)*planes_n);

				int * separation_time;
				separation_time
				break;

		}
	}


        close(input_file);


}
