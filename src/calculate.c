#include "calculate.h"
#include "station.h"
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
            /* Finds the connection which leads to the next station in the array. */
            if (stations[i].connections[j].station->id == stations[i+1].id) {
                sum_of_prices += stations[i].connections[j].route->price;
                /* Checks if the travel is by train and if it is the first train station in the journey.
                   If so, it adds 2.5 euros (starting price for a train journey) to the price sum. */
                if (stations[i].connections[j].route->type == RAIL && i != 0) {
                    if (stations[i-1].connections[j].route->type != RAIL) {
                        sum_of_prices += (float) 2.5;
                    }
                }
                /* Like the above check, however, this only checks the very first station. */
                else if (stations[i].connections[j].route->type == RAIL && i == 0) {
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
            /* Finds the connection which leads to the next station in the array. */
            if (stations[i].connections[j].station->id == stations[i+1].id) {
                /* Checks whether the found connection is a train or a plane route,
                   then multiplies the route distance with a statically defined CO2 per km.  */
                switch(stations[i].connections[j].route->type) {
                    case AIR:
                        result += (float) (stations[i].connections[j].route->distance) * AIR_CO2_PER_KM;
                        break;
                    case RAIL:
                        result += (float) (stations[i].connections[j].route->distance) * RAIL_CO2_PER_KM;
                        break;
                    default:
                        /* In case of unexpected travel type, it terminates the program with an error message. */
                        printf("TravelType is neither AIR nor RAIL.");
                        exit(EXIT_FAILURE);
                }
            }
        }
    }

    return result;
}
