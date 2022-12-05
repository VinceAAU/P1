#pragma once
#include <stdio.h>
#include "station.h"

// char * input_from_user();
char* start_station_from_user (Station* station);

char* end_station_from_user (Station* station);

int validate_input(int user_input_station, Station station);