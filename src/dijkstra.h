#pragma once
#include "station.h"

int* create_adjacency_matrix_for_dijkstra_algorithm(int number_of_stations, Station* station_array, int allow_planes_bool);


Station* calculate_optimal_route(int* G, int startnode,int endnode, int number_of_stations, Station* station_array, int* route_length, int current_time);

void debug_print_stations(int number_of_stations, Station* statio_array);

void print_matrix(int size, int*matrix);

Station* debugging_data(int size);