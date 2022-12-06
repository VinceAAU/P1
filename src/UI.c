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
        printf("Train: Travel time: %lf minutes, Co2 emission: %lf kg, estimated kost: %lf kr.\n", train_sum_of_time, train_co2_emitted, train_sum_of_price);
        printf("Plane: Travel time: %lf minutes, Co2 emission: %lf kg, estimated kost: %lf kr.\n", plane_sum_of_time, plane_co2_emitted, plane_sum_of_price);
    }else{
        printf("Plane: Travel time: %lf minutes, Co2 emission: %lf kg, estimated kost: %lf kr.\n", plane_sum_of_time, plane_co2_emitted, plane_sum_of_price);
        printf("Train: Travel time: %lf minutes, Co2 emission: %lf kg, estimated kost: %lf kr.\n", train_sum_of_time, train_co2_emitted, train_sum_of_price);
    }

}

