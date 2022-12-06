/*
 * TODO: Reset smartly after invalid input from user
 * TODO: Finish validate input
 * TODO: How should i output the two stations?
 */

#include "UI.h"
#include "station.h"
#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
/*
char* input_from_user ()
{
    char user_input_start_station [100],
         user_input_end_station [100];

    printf("Where does your journey start from\n");
    scanf("%s ", user_input_start_station);


    // check if the station is valid. If invalid ask for first station again
    if (validate_input(user_input_start_station, Station station) == 1) {
        printf("Where would you like to go?\n");
        scanf("%s ", user_input_end_station);


        // Check if destination station is valid. If invalid ask for destination station again
        if (validate_input(user_input_end_station, Station station) == 0){
            printf("invalid input\n");

            // do something to get second station again that is not stupid
        }

    }
    else {
        printf("Invalid station\n");

        // do something to get first station again that is not stupid
    }
}
*/

Station* start_station_from_user (Station* station){
    char user_input_start_station;
    bool station_is_valid = false;

    while (station_is_valid == false){
        printf("Where does your journey start from\n");
        scanf("%s ", &user_input_start_station);

        // checking if the station is in our list and returning a true if the station is present
        station_is_valid = (bool)get_station_by_id(station, user_input_start_station);
    }

    // returns the station by id
    return get_station_by_id(station, user_input_start_station);
}

Station* end_station_from_user (Station* station){
    char user_input_end_station;
    bool station_is_valid = false;

    while (station_is_valid == false){
        printf("Where does your journey end\n");
        scanf("%s ", &user_input_end_station);

        // checking if the station is in our list and returning a true if the station is present
        station_is_valid = (bool)get_station_by_id(station, user_input_end_station);
    }

    // returns the station by id
    return get_station_by_id(station, user_input_end_station);
}


// Function for checking whether the input station is valid or invalid
int validate_input(int user_input_station, Station station){

    int number_of_stations = sizeof(station.id);

    for (int i = 0; i < number_of_stations; i++){

        if (station.id == user_input_station) {
            return 1;
        }
        else {
            return 0;
        }
    }
}


void output_result(int train_sum_of_time, float train_co2_emitted, float train_sum_of_price,
                   int plane_sum_of_time, float plane_co2_emitted, float plane_sum_of_price,
                   int preference){
    int winner = -1;
    printf("For your preference you should choose:");
    switch (preference) {
        case 0: //time
            if(train_sum_of_time < plane_sum_of_time){
                printf(" Train\n");
                winner = 0;
            }else{
                printf(" Plane\n");
                winner = 1;
            }
            break;
        case 1: //CO2
            if(train_co2_emitted < plane_co2_emitted){
                printf(" Train\n");
                winner = 0;
            }else{
                printf(" Plane\n");
                winner = 1;
            }
            break;
        case 2: //price
            if(train_sum_of_price < plane_sum_of_price){
                printf(" Train\n");
                winner = 0;
            }else{
                printf(" Plane\n");
                winner = 1;
            }
            break;
        default:
            printf("\nError\n");
            break;
    }

    if(winner == 0){
        printf("Train: Travel time: %d minutes, Co2 emission: %lf kg, estimated kost: %lf kr.\n", train_sum_of_time, train_co2_emitted, train_sum_of_price);
        printf("Plane: Travel time: %d minutes, Co2 emission: %lf kg, estimated kost: %lf kr.\n", plane_sum_of_time, plane_co2_emitted, plane_sum_of_price);
    }else{
        printf("Plane: Travel time: %d minutes, Co2 emission: %lf kg, estimated kost: %lf kr.\n", plane_sum_of_time, plane_co2_emitted, plane_sum_of_price);
        printf("Train: Travel time: %d minutes, Co2 emission: %lf kg, estimated kost: %lf kr.\n", train_sum_of_time, train_co2_emitted, train_sum_of_price);
    }

}

