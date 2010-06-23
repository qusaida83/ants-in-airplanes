void init_possible_times(){
	int j;
	possible_times_size = airplanes[current_plane].latest_lt - airplanes[current_plane].earliest_lt + 1;
	//printf("possible times size = %d\n",possible_times_size);
	possible_times = (char *)malloc( sizeof(char)*possible_times_size );
	//puts("alloked");
	for (j = 0; j < possible_times_size; j++) {
		possible_times[j] = '0';
	}
	/*
	puts("Bitmap:");
	for (j = 0; j < possible_times_size; j++) {
		printf("%c",possible_times[j]);
	}
	puts("");
	*/
}
void fill_possible_times(){
	unsigned int j;
	int k;
	for(j=0; j<range.num_elem; j++){
		int for_init;
		int for_end;

		if( range.ranges[range.last_read_pos+1] < airplanes[current_plane].earliest_lt) {
			for_init=0;
			for_end = -1;
			//out of scope
		}else if(range.ranges[range.last_read_pos] > airplanes[current_plane].latest_lt){
			for_init=0;
			for_end = -1;
			//out of scope
		}else{
			if(range.ranges[range.last_read_pos] > airplanes[current_plane].earliest_lt){
				for_init = range.ranges[range.last_read_pos] - airplanes[current_plane].earliest_lt;
			}else{
				for_init = 0;
			}
			if(range.ranges[range.last_read_pos+1] < airplanes[current_plane].latest_lt){
				for_end = range.ranges[range.last_read_pos+1] - airplanes[current_plane].earliest_lt;
			}else{
				for_end = possible_times_size;
			}
		}
		
		//printf("For range: %d --- %d\n", for_init, for_end);
		
		//printf("For init: %d   | for end: %d\n",for_init,for_end);
		for(k=for_init; k<=for_end; k++)
			possible_times[k]='1';
		range.last_read_pos+=2;
	}
	/*
	puts("Bitmap:");
	for (j = 0; j < possible_times_size; j++) {
		printf("%c",possible_times[j]);
	}
	puts("");
	*/
}
