#include "station.h"
#include "calculate.h"
#include "UI.h"
#include "JSON-interface.h"
#include "dijkstra.h"
#include <stdlib.h>
#include <stdio.h>


int main(void) {
    int start_time = 0, preference = -1;
    Station* stations = retrieve_JSON_data("data.json");

    size_t number_of_stations = station_list_length(stations);
    printf("These are all the stations you can choose from: \n");
    print_stations(number_of_stations, stations);

    start_station_from_user(stations);
    end_station_from_user(stations);
    start_time = journey_start_time_from_user();
    preference = select_preference();


    Station* optimal_train_route = run_dijkstra(stations);
    Station* optimal_plane_route = run_dijkstra(stations);




}
