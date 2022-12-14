#pragma once
#include <time.h>

//We define this at the start, because Connection and Station depend on each other
struct Station;
typedef struct Station Station;

typedef int ID;

typedef enum {
    AIR,
    RAIL
} TravelType;

typedef struct {
    //The numbers on node1 and node2 do not correspond to start/end, but are rather alphabetical
    Station* node1;
    Station* node2;
    ID line;
    float price;
    int distance;
    int duration; // In seconds
    TravelType type;
} Route;

typedef struct {
    Station* station;
    Route* route;
    int* timetable; //Number of seconds after midnight
    size_t timetable_length;
} Connection;

typedef struct Station {
    char* name;
    ID id;
    Connection* connections;
} Station;

/*
 * Returns length of the station list
 */
size_t station_list_length(Station*);

Station* get_station_by_id(Station* station_list, ID id);

size_t connection_list_length(Connection* connection_list);