#pragma once
#include <stdio.h>
#include "station.h"

Station* start_station_from_user (Station* station);

Station* end_station_from_user (Station* station);

Station* journey_start_time_from_user(Station* station);

int validate_input(int user_input_station, Station station);

void output_result(int train_sum_of_time, float train_co2_emitted, float train_sum_of_price,
                   int plane_sum_of_time, float plane_co2_emitted, float plane_sum_of_price,
                   int preference);