#include <string.h>
#include "station.h"

// The last element in a station array must be filled with zeros.
// If the last element is not zero, this function will probably cause a segfault
// TODO: Write tests for this
size_t station_list_length(Station* station_list){
    if(station_list == NULL){
        return 0;
    }

    size_t i = 0;
    while(station_list[i].name != NULL){
        i++;
    }

    return i;
}
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

// Station list is not sorted, so we use a linear search, making this
// function quite slow.
Station *get_station_by_id(Station *station_list, ID id) {
    for (int i = 0; i < station_list_length(station_list); i++) {
        if (station_list[i].id == id) {
            return station_list + i;
        }
    }
    return NULL; //If no station is found
}

Station *get_station_by_name(Station *station_list, char *name) {
    //If two stations have the same name, this will just return the first
    for (int i = 0; i < station_list_length(station_list); i++) {
        if (strcmp(station_list[i].name, name) == 0) {
            return &station_list[i];
        }
    }
    return NULL;
}