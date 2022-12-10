#pragma once
#include "station.h"

size_t connection_list_length(Connection*);
int string_to_seconds(char* string);
float calculate_price(Station*);
float calculate_co2(Station*);