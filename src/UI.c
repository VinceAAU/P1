/*
 * TODO: Reset smartly after invalid input from user
 * TODO: Finish validate input
 * TODO: How should i output the two stations?
 */

#include "UI.h"
#include "station.h"
#include <stdio.h>
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

int journey_start_time_from_user() {
    int hours = 0, minutes = 0;

    printf("At what time would you like to start your journey? \n"
           "(Please write in HH:MM format \n)");
    scanf("%d:%d", &hours, &minutes);

    return hours * 24 + minutes * 60;
}

/**
 * Havde ingen ide om hvordan den tidligere kode fungerede, så lavede bare noget der burde virke.
 * @param preference
 * @return
 */
int Select_preference(int preference) {
    printf("What would you like the program to prioritise \n"
           "Write: '0' for Time \n"
           "Write: '1' for Climate \n"
           "Write: '2' for Price \n" );
    scanf("%d", &preference);

    return preference;
}

/*
 * TODO: Add change of train/plane at which stations
 */
void output_result(Station* train_stations, int train_sum_of_time, float train_co2_emitted, float train_sum_of_price,
                   Station* plane_stations, int plane_sum_of_time, float plane_co2_emitted, float plane_sum_of_price,
                   int preference){
    int best_transport_mode = -1, train_hours = 0, train_minutes = 0, plane_hours = 0, plane_minutes = 0;

    printf("For your preference you should choose:");
    switch (preference) {
        case 0: //time
            if(train_sum_of_time < plane_sum_of_time){
                printf(" Train\n");
                best_transport_mode = 0;
            }else{
                printf(" Plane\n");
                best_transport_mode = 1;
            }
            break;
        case 1: //CO2
            if(train_co2_emitted < plane_co2_emitted){
                printf(" Train\n");
                best_transport_mode = 0;
            }else{
                printf(" Plane\n");
                best_transport_mode = 1;
            }
            break;
        case 2: //price
            if(train_sum_of_price < plane_sum_of_price){
                printf(" Train\n");
                best_transport_mode = 0;
            }else{
                printf(" Plane\n");
                best_transport_mode = 1;
            }
            break;
        default:
            printf("\nError\n");
            break;
    }

    train_minutes = train_sum_of_time/60;
    train_hours = train_minutes/60;
    train_minutes = train_minutes%60;


    plane_minutes = plane_sum_of_time/60;
    plane_hours = plane_minutes/60;
    plane_minutes = plane_minutes%60;
/*
    for(int i = 0;i < 1000 ; i++){
        if (i == 0) {

        }else if(stations[i] == NULL){
            i = 10000;
        }else if (stations[i-1].connections[j].route->line != stations[i].connections[j].route->line) {

        }
    }
*/

    if(best_transport_mode == 0){
        printf("Train: Travel time: %d hours and %d minutes, Co2 emission: %lf kg, estimated cost: %lf kr.\n", train_hours, train_minutes, train_co2_emitted, train_sum_of_price);
        printf("Plane: Travel time: %d hours and %d minutes, Co2 emission: %lf kg, estimated cost: %lf kr.\n", plane_hours, plane_minutes, plane_co2_emitted, plane_sum_of_price);
    }else{
        printf("Plane: Travel time: %d hours and %d minutes, Co2 emission: %lf kg, estimated cost: %lf kr.\n", plane_hours, plane_minutes, plane_co2_emitted, plane_sum_of_price);
        printf("Train: Travel time: %d hours and %d minutes, Co2 emission: %lf kg, estimated cost: %lf kr.\n", train_hours, train_minutes, train_co2_emitted, train_sum_of_price);
    }

}

