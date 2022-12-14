#include "station.h"
#include "calculate.h"
#include "UI.h"
#include "JSON-interface.h"
#include "dijkstra.h"
#include <stdlib.h>
#include <stdio.h>


int main(void) {
    // Declaring and initialising variables and extracting JSON data.
    int start_time = 0, preference = -1, train_sum_time = -1, plane_sum_time = -1;
    float train_co2_emitted = 0, plane_co2_emitted = 0, train_sum_of_price = 0, plane_sum_of_price = 0;
    Station* stations = retrieve_JSON_data("data.json");

    // Output list of all stations for the user to read.
    size_t number_of_stations = station_list_length(stations);
    printf("These are all the stations you can choose from: \n");
    print_stations(number_of_stations, stations);

    // Getting input from the user.
    Station* start_station = start_station_from_user(stations);
    Station* end_station = end_station_from_user(stations);
    start_time = journey_start_time_from_user();
    preference = select_preference();

    // Finding both the optimal train and plane route.
    Station* optimal_train_route = run_dijkstras(number_of_stations, stations, RAIL, start_time, &train_sum_time, start_station->id, end_station->id);
    Station* optimal_plane_route = run_dijkstras(number_of_stations, stations, AIR, start_time, &plane_sum_time, start_station->id,end_station->id);

    // Calculating the price and CO2 emitted for the train route.
    train_sum_of_price = calculate_price(optimal_train_route);
    train_co2_emitted = calculate_co2(optimal_train_route);

    // Calculating the price and CO2 emitted for the plane route.
    plane_sum_of_price = calculate_price(optimal_plane_route);
    plane_co2_emitted = calculate_co2(optimal_plane_route);

    // Outputting the result for the user to read.
    output_result(optimal_train_route, train_sum_time, train_co2_emitted, train_sum_of_price,
                  optimal_plane_route, plane_sum_time, plane_co2_emitted, plane_sum_of_price,
                  preference);

}
