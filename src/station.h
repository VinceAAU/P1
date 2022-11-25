#pragma once
#include <time.h>

typedef char[] ID;

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
    struct tm[] timetable;
} Connection;

typedef struct {
    char[] name;
    ID id;
    Connection[] connections;
} Station;