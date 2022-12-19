#include "UI.h"
#include "station.h"
#include "calculate.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 100
Station* start_station_from_user (Station* station){
    char user_input_start_station[BUFFER_SIZE];
    bool station_is_valid = false;

    while (station_is_valid == false) {
        printf("Where does your journey start from? \n(Please write in 'Station X' format) \n");
        fgets(user_input_start_station, BUFFER_SIZE, stdin);
        user_input_start_station[strlen(user_input_start_station) - 1] = '\0';

        // checks if the station is in our list and returns a true if the station is present
        station_is_valid = (bool) get_station_by_name(station, user_input_start_station);
    }

    // returns the station by name
    return get_station_by_name(station, user_input_start_station);
}

Station* end_station_from_user (Station* station){
    char user_input_end_station[BUFFER_SIZE];
    bool station_is_valid = false;

    while (station_is_valid == false) {
        printf("Where does your journey end? \n(Please write in 'Station X' format) \n");
        fgets(user_input_end_station, BUFFER_SIZE, stdin);
        user_input_end_station[strlen(user_input_end_station) - 1] = '\0';

        // checks if the station is in our list and returns a true if the station is present
        station_is_valid = (bool) get_station_by_name(station, user_input_end_station);
    }

    // returns the station by name
    return get_station_by_name(station, user_input_end_station);
}

int journey_start_time_from_user() {
    int seconds = 0;
    char time[6];

    printf("At what time would you like to start your journey? \n"
           "(Please write in HH:MM format) \n");
    scanf("%5s", time);

    seconds = string_to_seconds(time);
    if (seconds == -1) {
        printf("Invalid start time, please try again. \n");
        seconds = journey_start_time_from_user();
    }

    return seconds;
}

/**
 * This function asks the user what they would preference when deciding on trains or planes
 * They would have to pick either 0,1 or 2 to choose time, climate or price
 * @param preference
 * @return
 */
int select_preference() {
    int preference = -1;
    printf("What would you like the program to prioritise? \n"
           "Write: '0' for Time \n"
           "Write: '1' for Climate \n"
           "Write: '2' for Price \n" );
    scanf("%d", &preference);

    if (!(preference >= 0 && preference <= 2)) {
        printf("Your input was invalid, please try again. \n");
        preference = select_preference();
    }

    return preference;
}
/**
 * This function takes in a station array and the length of the array and then prints all the stations in the array
 * @param number_of_stations
 * @param station_array
 */
void print_stations(size_t number_of_stations, Station* station_array) {
    for(int i = 0; i < number_of_stations; i++) {
        printf("Station %c \n", station_array[i].id);
    }
}

/**
 * This function handles outputting the optimal route to the user in a format that is easily readable
 * @param optimal_train_route
 * @param train_sum_of_time
 * @param train_co2_emitted
 * @param train_sum_of_price
 * @param optimal_plane_route
 * @param plane_sum_of_time
 * @param plane_co2_emitted
 * @param plane_sum_of_price
 * @param preference
 */
void output_result(Station* optimal_train_route, int train_sum_of_time, float train_co2_emitted, float train_sum_of_price,
                   Station* optimal_plane_route, int plane_sum_of_time, float plane_co2_emitted, float plane_sum_of_price,
                   int preference) {
    int best_transport_mode = -1, train_hours = 0, train_minutes = 0, plane_hours = 0, plane_minutes = 0, non_identical_route = -1;
    size_t number_of_train_stations = 0, number_of_plane_stations = 0;
    printf("For your preference you should choose:");
    //The switch checks which route is better according to the preference chosen early in the program
    switch (preference) {
        case 0: //time
            if (train_sum_of_time <= plane_sum_of_time) {
                printf(" Train\n");
                best_transport_mode = 0;
            } else {
                printf(" Plane\n");
                best_transport_mode = 1;
            }
            break;
        case 1: //CO2
            if (!(train_co2_emitted > plane_co2_emitted)) {
                printf(" Train\n");
                best_transport_mode = 0;
            } else {
                printf(" Plane\n");
                best_transport_mode = 1;
            }
            break;
        case 2: //price
            if (!(train_sum_of_price > plane_sum_of_price)) {
                printf(" Train\n");
                best_transport_mode = 0;
            } else {
                printf(" Plane\n");
                best_transport_mode = 1;
            }
            break;
        default:
            printf("\nError\n");
            break;
    }
    //converting seconds to hours and minutes
    train_minutes = train_sum_of_time / 60;
    train_hours = train_minutes / 60;
    train_minutes = train_minutes % 60;

    //converting seconds to hours and minutes
    plane_minutes = plane_sum_of_time / 60;
    plane_hours = plane_minutes / 60;
    plane_minutes = plane_minutes % 60;

    //finding number of stations on both the optimal routes, as it is used later
    number_of_train_stations = station_list_length(optimal_train_route);
    number_of_plane_stations = station_list_length(optimal_plane_route);
    //checks whether the two routes are identical, and only outputs the train route if that is the case
    non_identical_route = check_identical_route(number_of_train_stations, optimal_train_route, optimal_plane_route);
    if (non_identical_route) {
        if (best_transport_mode == 0) {
            print_optimal_train_route(number_of_train_stations, optimal_train_route, train_hours, train_minutes,
                                      train_co2_emitted, train_sum_of_price);

            print_optimal_plane_route(number_of_plane_stations, optimal_plane_route, plane_hours, plane_minutes,
                                      plane_co2_emitted, plane_sum_of_price);
        } else if (best_transport_mode == 1) {
            print_optimal_plane_route(number_of_plane_stations, optimal_plane_route, plane_hours, plane_minutes,
                                      plane_co2_emitted, plane_sum_of_price);

            print_optimal_train_route(number_of_train_stations, optimal_train_route, train_hours, train_minutes,
                                      train_co2_emitted, train_sum_of_price);

        } else {
            printf("Could not find route from preference.\n Train and plane route will simply be output\n");

            print_optimal_train_route(number_of_train_stations, optimal_train_route, train_hours, train_minutes,
                                      train_co2_emitted, train_sum_of_price);

            print_optimal_plane_route(number_of_plane_stations, optimal_plane_route, plane_hours, plane_minutes,
                                      plane_co2_emitted, plane_sum_of_price);

        }
    }else{
        printf("There is no viable route by plane. \n");
        print_optimal_train_route(number_of_train_stations, optimal_train_route, train_hours, train_minutes,
                                  train_co2_emitted, train_sum_of_price);
    }
}
/**
 * This function handles outputting the optimal train route, so that it is not repeated in the code
 * @param number_of_train_stations //needed for printing all the stations in the optimal route with print_stations function
 * @param optimal_train_route
 * @param train_hours
 * @param train_minutes
 * @param train_co2_emitted
 * @param train_sum_of_price
 */
void print_optimal_train_route(size_t number_of_train_stations, Station* optimal_train_route, int train_hours, int train_minutes, float train_co2_emitted, float train_sum_of_price){
    printf("Train: Travel time: %d hours and %d minutes, Co2 emission: %.3lf kg, estimated cost: %.2lf EUR.\n", train_hours,
           train_minutes, train_co2_emitted, train_sum_of_price);
    printf("These are all the stations on your optimal train route: \n");
    print_stations(number_of_train_stations, optimal_train_route);
}
/**
 *  This function handles outputting the optimal plane route, so that it is not repeated in the code
 * @param number_of_plane_stations //needed for printing all the stations in the optimal route with print_stations function
 * @param optimal_plane_route
 * @param plane_hours
 * @param plane_minutes
 * @param plane_co2_emitted
 * @param plane_sum_of_price
 */
void print_optimal_plane_route(size_t number_of_plane_stations, Station* optimal_plane_route, int plane_hours, int plane_minutes, float plane_co2_emitted, float plane_sum_of_price){
    printf("Plane: Travel time: %d hours and %d minutes, Co2 emission: %.3lf kg, estimated cost: %.2lf EUR .\n", plane_hours, plane_minutes, plane_co2_emitted, plane_sum_of_price);
    printf("These are all the stations on your optimal plane route: \n");
    print_stations(number_of_plane_stations, optimal_plane_route);
}
/**
 * Checks whether the two routes are identical
 * @param number_of_train_stations //used for running the for loop, so that it can return false if they are identical
 * @param optimal_train_route
 * @param optimal_plane_route
 * @return
 */
int check_identical_route(size_t number_of_train_stations, Station* optimal_train_route, Station* optimal_plane_route){
    for(int i = 0; i < number_of_train_stations; i++){
        if(strcmp(optimal_plane_route[i].name, optimal_train_route[i].name) != 0){
            return 1;
        }
    }
    return 0;
}
