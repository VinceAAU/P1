#pragma once
#include "station.h"

int* create_adjacency_matrix_for_dijkstra_algorithm(size_t number_of_stations, Station* station_array, int allow_planes_bool);

Station* calculate_optimal_route(int* adjacency_matrix, int startnode,int endnode, size_t number_of_stations, Station* station_array, int current_time, int* output_time);

void debug_print_stations(int number_of_stations, Station* statio_array);

void print_matrix(int size, int*matrix);

Station* debugging_data(int size);

Station* run_dijkstras(size_t total_number_of_stations, Station* all_stations, TravelType travel_type, int current_time, int* output_time, ID start_station, ID end_station);