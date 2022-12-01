#include "station.h"

// The last element in a station array must be filled with zeros.
// If the last element is not zero, this function will probably cause a segfault
// TODO: Write tests for this
size_t station_list_length(Station* station_list){
    if(station_list == NULL){
        return 0;
    }

    int i = 0;
    while(station_list[i].name != NULL){
        i++;
    }

    return i;
}