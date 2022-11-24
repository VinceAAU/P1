#include <time.h>

typedef char* ID;

typedef enum{
	AIR,
	RAIL
} TravelType;

typedef struct {
	Connection* timetable;
	char* name;
	ID id;
} Station;

typedef struct {
	struct tm departure;
	int journey_time; //Journey time in seconds
	float price; //TODO: Decide if we use ints or floats
	int distance; //Distance in km
	Station* destination;
	ID id;
} Connection;

typedef struct {
	int time;
	int price;
	int emissions; //In kg of CO2e 
} Route;

