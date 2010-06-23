
struct ranges_list{
	unsigned int num_elem;
	long int last_written_pos;
	long int last_read_pos;
	long int * ranges;
};

void init_range_list(){
	range.num_elem = 0;
	range.last_written_pos = 0;
	range.last_read_pos = 0;
	range.ranges = (long int *)malloc(sizeof(long int)*planes_n*2);
}


void write_ranges(int i){

	unsigned int j;
	//gets all ranges from already landed planes.
	for(j=0; j<planes_n; j++){ // for each other plane ...
		//printf("Airplane %d, lt %ld\n",j, ants[i].planes_lt[j]);
		if( ants[i].planes_lt[j] > -1){ // ... that was already visited
			//some alias to help
			int min_range = ants[i].planes_lt[j] - separation_time[current_plane][j];
			int max_range = ants[i].planes_lt[j] + separation_time[current_plane][j];

			if(min_range < 0){
				min_range = 0;
			}

			range.ranges[range.last_written_pos] = min_range;
			range.ranges[range.last_written_pos+1] = max_range;
			range.last_written_pos += 2;
			range.num_elem++;
		} // end to analize the conflict time with this plane.
	}
/*
	puts("RANGES");
	for(j=0; j<range.num_elem; j++){
		printf("%ld ========= %ld\n",range.ranges[range.last_read_pos], range.ranges[range.last_read_pos+1]);
		range.last_read_pos+=2;
	}
	range.last_read_pos = 0;
*/
}
