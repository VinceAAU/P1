/*
 * TODO: Reset smartly after invalid input from user
 * TODO: Finish validate input
 * TODO: How should i output the two stations?
 */

#include "UI.h"
#include "station.h"
#include <stdio.h>

char* input_from_user ()
{
    char user_input_start_station [100],
         user_input_end_station [100];

    printf("Where does your journey start from\n");
    scanf("%s ", %user_input_start_station);


    // check if the station is valid. If invalid ask for first station again
    if (validate_input(user_input_start_station) == 1) {
        printf("Where would you like to go?\n");
        scanf("%s ", %user_input_end_station);


        // Check if destination station is valid. If invalid ask for destination station again
        if (validate_input(user_input_end_station) == 0){
            printf("invalid input\n");

            // do something to get second station again that is not stupid
        }

    }
    else {
        printf("Invalid station\n");

        // do something to get first station again that is not stupid
    }



}

// Function for checking whether the input station is valid or invalid
int validate_input(int user_input_station, Station station){

    int number_of_stations = sizeof(*station.id);

    for (int i = 0; i < station_list_length(); i++){

        if (*station.id[i] == user_input_station) {
            return 1;
        }
        else {
            return 0;
        }
    }
}