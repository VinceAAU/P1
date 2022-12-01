#pragma once
#include "station.h"

int* create_adjacency_matrix_for_dijkstra_algorithm(int number_of_stations, Station* station_array);


void calculate_optimal_route();


void print_matrix(int size, int*matrix);

Station* debugging_data();