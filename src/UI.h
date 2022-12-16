#pragma once
#include <stdio.h>
#include "station.h"

Station* start_station_from_user (Station* station);

Station* end_station_from_user (Station* station);

int journey_start_time_from_user();
int select_preference();

int validate_input(int user_input_station, Station station);

void output_result(Station* train_stations, int train_sum_of_time, float train_co2_emitted, float train_sum_of_price,
                   Station* plane_stations, int plane_sum_of_time, float plane_co2_emitted, float plane_sum_of_price,
                   int preference);
void print_stations(size_t number_of_stations, Station* station_array);

void print_optimal_train_route(size_t number_of_train_stations, Station* optimal_train_route, int train_hours, int train_minutes, float train_co2_emitted, float train_sum_of_price);


void print_optimal_plane_route(size_t number_of_plane_stations, Station* optimal_plane_route, int plane_hours, int plane_minutes, float plane_co2_emitted, float plane_sum_of_price);

int check_optimal_route(size_t number_of_train_stations, Station* optimal_train_route, Station* optimal_plane_route);