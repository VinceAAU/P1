#include "calculate.h"
#include "station.h"
#include <stdlib.h>
#include <string.h>

// TODO: Pick a solution based on what the input will look like.

// Ripped from station.c adjusted to use connection-structs.

size_t connection_list_length(Connection* connection_list){
    if(connection_list == NULL){
        return 0;
    }

    size_t i = 0;
    while(connection_list[i].station[0].name != NULL){
        i++;
    } // TODO: Ensure functionality. And probably move/delete.

    return i;
}

// Ripped from station.c adjusted to use route-structs.

size_t route_list_length(Route* route_list){
    if(route_list == NULL){
        return 0;
    }

    size_t i = 0;
    while(route_list[i].node1[0].name != NULL){
        i++;
    } // TODO: Ensure functionality. And probably move/delete.

    return i;
}

/*
 * If the station-array includes all the stations used in the optimal route,
 * but these station-structs have not been altered, i.e.
 * they still have all their connections and routes.
 *
 * Assuming route.line follows the current Example.json's structure of connection "E-I": "routeID": "EI",
 * meaning route.ID is actually a char* not an int, then:
 */

float calculate_price(Station* station) {
    if(station == NULL){
        return 0;
    }

    float sum_of_prices = 0;
    size_t arr_len = station_list_length(station);

    for (size_t i = 0; i < arr_len; ++i) {
        char *desired_id = calloc(3, sizeof(char));
        strcat(desired_id, station[i].id); // Decide which it is; type INT or CHAR (Probably CHAR).
        strcat(desired_id, station[i + 1].id);
        desired_id[2] = '\0';

        size_t con_len = connection_list_length(station[i].connections);
        for (int j = 0; j < con_len; ++j) {
            size_t route_len = route_list_length(station[i].connections[j].route);
            for (int k = 0; k < route_len; ++k) {
                if (strcmp(station[i].connections[j].route[k].line, desired_id)) { //strcomp -- assuming it's char.
                    sum_of_prices += station[i].connections[j].route[k].price;
                }
            }
        }
    }
    return sum_of_prices;
}

float calculate_co2(Station* station){
    if(station == NULL){
        return 0;
    }

    int dist_sum = 0;
    size_t arr_len = station_list_length(station);

    for (size_t i = 0; i < arr_len; ++i) {
        char *desired_id = calloc(3, sizeof(char));
        strcat(desired_id, station[i].id); // Decide which it is; type INT or CHAR.
        strcat(desired_id, station[i + 1].id);
        desired_id[2] = '\0';

        size_t con_len = connection_list_length(station[i].connections);
        for (int j = 0; j < con_len; ++j) {
            size_t route_len = route_list_length(station[i].connections[j].route);
            for (int k = 0; k < route_len; ++k) {
                if (strcmp(station[i].connections[j].route[k].line, desired_id)) {
                    dist_sum += station[i].connections[j].route[k].distance;
                }
            }
        }
    }
    float co2_emitted = dist_sum * CO2_PER_KM;
    //TODO: Define a symbolic constant for CO2 per km.
    return co2_emitted;
}


/*
 * If the given station-array only has the stations on the optimal route
 * and the stations only contain the used connection and the used route:

float calculate_price(Station* station) {
    if(station == NULL){
        return 0;
    }

    float sum_of_prices = 0;
    size_t arr_len = station_list_length(station);

    for (size_t i = 0; i < arr_len; ++i) {
        sum_of_prices += station[i].connections[0].route[0].price;
    }

    return sum_of_prices;
}

float calculate_co2(Station* station){
    if(station == NULL){
        return 0;
    }

    int dist_sum = 0;
    size_t arr_len = station_list_length(station);

    for (size_t i = 0; i < arr_len; ++i) {
        dist_sum += station[i].connections[0].route[0].distance;
    }

    float co2_emitted = dist_sum * CO2_PER_KM;
    //TODO: Define a symbolic constant for CO2 per km.

    return co2_emitted;
} */