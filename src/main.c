#include "station.h"
#include "calculate.h"
#include <stdlib.h>
#include <stdio.h>

Station* debugging_data()
{
    Route *route = malloc(sizeof(Route));
    Station *stations =  malloc(5 * sizeof(Station));
    Connection *connections = malloc(3 * sizeof(Connection));

    Station foo = (Station){0};
    Connection poo = (Connection) {0};

    for(int i = 0; i < 2; i++)
    {
        route->duration = i+1;
        connections[i].route = route;
        connections[i].station = &stations[i];
        connections[i].station->name = "Brr";
        connections[i].station->id = i+3;
        connections[i].route->price = 5;
        connections[i].route->distance = 8;
    } connections[2] = poo;

    for(int i = 0; i < 5; i++)
    {
        stations[i].connections = connections;
        stations[i].name = "Siooo";
    }  stations[5] = foo;

    for(int i = 0; i < 5; i++)
    {
        stations[i].id = i;
    }

    return stations;
}

int main(void) {
    printf("%f \n", calculate_price(debugging_data()));
    printf("%f \n", calculate_co2(debugging_data()));
}
