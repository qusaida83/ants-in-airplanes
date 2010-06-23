
struct ranges_list{
	int num_elem;
	int last_written_pos;
	int last_read_pos;
	int * ranges;
};

init_range_list(){
	range.num_elem = 0;
	range.last_written_pos = 0;
	range.last_read_pos = 0;
	range.ranges = (int *)malloc(sizeof(int)*planes_n*2);
}


void write_ranges(int i){

	int j;
	//gets all ranges from already landed planes.
	for(j=0; j<planes_n; j++){ // for each other plane ...
		if( ants[i].planes_lt[j] > 0){ // ... that was already visited
			//some alias to help
			int min_range = ants[i].planes_lt[j] - separation_time[current_plane][j];
			int max_range = ants[i].planes_lt[j] + separation_time[current_plane][j];
			range.ranges[range.last_written_pos] = min_range;
			range.ranges[range.last_written_pos+1] = max_range;
			range.last_written_pos += 2;
			range.num_elem++;
		} // end to analize the conflict time with this plane.
	}
}
