#include "calculate.h"
#include "station.c"
#include <stdlib.h>
#include <stdio.h>

#define AIR_CO2_PER_KM (float)88.8
#define RAIL_CO2_PER_KM (float)21.5


/**
 * Takes the time and splits them up in hours and minutes
 * Converts the hours and minutes to seconds and adds them into the seconds variable
 */
int string_to_seconds(char *string) {
    int hours, minutes, seconds;

    sscanf(string, "%d:%d", &hours, &minutes);
    if (!(hours < 24 && hours >= 0 && minutes < 60 && minutes >= 0)) {
        return -1;
    }

    seconds = (hours * 3600) + (minutes * 60);

    return seconds;
}

/**
 * Takes the sum of all the prices on the route which has been selected as the optimal route.
 * @param stations The array of stations that contain the optimal route.
 * @return The function returns the price of the journey.
 */
float calculate_price(Station* stations) {
    float sum_of_prices = 0;
    size_t stations_length = station_list_length(stations);

    for (size_t i = 0; i < stations_length; ++i) {
        size_t connections_length = connection_list_length(stations[i].connections);
        for (size_t j = 0; j < connections_length; ++j) {
            if (stations[i].connections[j].station->id == stations[i+1].id) {
                sum_of_prices += stations[i].connections[j].route->price;
                if (stations[i].connections[j].route->type == RAIL && i != 0) {
                    if (stations[i-1].connections[j].route->type != RAIL) {
                        sum_of_prices += (float) 2.5;
                    }
                } else if (stations[i].connections[j].route->type == RAIL && i == 0) {
                    sum_of_prices += (float) 2.5;
                }
            }
        }
    }

    return sum_of_prices;
}

/**
 * Takes the distance of each route in the optimal path, then multiplies it by a predefined
 * CO2/km amount and sums it into one result.
 * @param stations The array of stations that contain the optimal route.
 * @return The function returns the estimated CO2 emission of the journey.
 */
float calculate_co2(Station* stations){
    float result = 0;
    size_t stations_length = station_list_length(stations);

    for (size_t i = 0; i < stations_length; ++i) {
        size_t connections_length = connection_list_length(stations[i].connections);
        for (size_t j = 0; j < connections_length; ++j) {
            if (stations[i].connections[j].station->id == stations[i+1].id) {
                switch(stations[i].connections[j].route->type) {
                    case AIR:
                        result += (float) (stations[i].connections[j].route->distance) * AIR_CO2_PER_KM;
                        break;
                    case RAIL:
                        result += (float) (stations[i].connections[j].route->distance) * RAIL_CO2_PER_KM;
                        break;
                    default:
                        printf("TravelType is neither AIR nor RAIL.");
                        exit(EXIT_FAILURE);
                }
            }
        }
    }

    return result;
}
