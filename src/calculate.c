#include "calculate.h"
#include "station.c" //Probably need to do some CMake stuff if we don't want to include the .c files.
#include <stdlib.h>
#include <stdio.h>

#define AIR_CO2_PER_KM (float)3.14
#define RAIL_CO2_PER_KM (float)0.14

/**
 * Find the number of connections a station has.
 * @param connection_list The array of connections you wish to count.
 * @return Counting the length of the array and returning the length.
 */
size_t connection_list_length(Connection* connection_list) {
    if(connection_list == NULL){
        return 0;
    }

    size_t i = 0;
    while(connection_list[i].station != NULL){
        i++;
    }

    return i;
}

/**
 * Takes the time and splits them up in hours and minutes
 * Converts the hours and minutes to seconds and adds them into the seconds variable
 */
int string_to_seconds(char* string) {
int hours, minutes, seconds;
//Etellerandet skal ændres senere, det var bare for at teste
char etellerandet[] = "15;16";

sscanf(etellerandet, "%d %*c %d", &hours, &minutes);

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
    size_t arr_len = station_list_length(stations);

    for (size_t i = 0; i < arr_len; ++i) {
        size_t connections = connection_list_length(stations[i].connections);
        for (size_t j = 0; j < connections; ++j) {
            if (stations[i].connections[j].station->id == stations[i+1].id) {
                sum_of_prices += stations[i].connections[j].route->price;
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
    size_t arr_len = station_list_length(stations);
    for (size_t i = 0; i < arr_len; ++i) {
        size_t connections = connection_list_length(stations[i].connections);
        for (size_t j = 0; j < connections; ++j) {
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

    //Set the actual value of AIR_CO2_PER_KM and RAIL_CO2_PER_KM to the desired values.
    return result;
}