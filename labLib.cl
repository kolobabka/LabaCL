__average_val = func (arr) {

 	result = 0;

 	i = 0;
// 	arr_size = sizeof (arr);
 	//while (i != arr_size) {
 	//	result += get(arr, i);
 	//	i = i + 1;
 	//}

 	//result = result / arr_size;

}

__average_val_range = func (arr, st, fin) {

 	result = 0;
 	i = st;
 	while (i != fin) {
 		result += get(arr, i);
 		i = i + 1;
 	}

 	result = result / (fin - st);

}

__root_mean_square_error = func (arr) {

	numerator = 0;

	average_val = __average_val (arr);
	arr_size = sizeof (arr);
	i = 0;
	while (i != arr_size) {
		numerator = numerator  + (get(arr, i) - average_val) * (get(arr, i) - average_val);
		i = i + 1;
	}
	
	//result = sqrt (numerator / (arr_size - 1));
    result = 1;

}

