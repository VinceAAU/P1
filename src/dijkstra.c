/*
 * TODO: Fix debugging commands to properly test
 * TODO: If JSON checker doesn't order the route, the adjacency matrix function will have to. See line 42
 * TODO: Write function to check through routes for something
 */
#include "dijkstra.h"
#include <stdio.h>
#include <malloc.h>
#include "station.h"

int get_number_of_connections(Station station)
{

    return (sizeof(*station.connections)/sizeof(Connection));
}

int* create_adjacency_matrix_for_dijkstra_algorithm(int number_of_stations, Station* station_array ) {
    int (*adjacency_matrix)[number_of_stations] = malloc(sizeof(int[number_of_stations][number_of_stations]));

    // fill matrix with zeros, so stations are disconnected by default.
    for (int j = 0; j < number_of_stations; j++) {
        for (int i = 0; i < number_of_stations; i++) {
            adjacency_matrix[i][j] = 0;
        }
    }

    // allocates for array of the same size
    Station *array = (Station *) malloc(number_of_stations * sizeof(Station));

    // copies in elements from first array to second, cause apparently you have to do that in C
    for (int i = 0; i < number_of_stations; i++) {
        array[i] = station_array[i];
        array[i].id = i; // changes id to array position
    }

    for (int row = 0; row < number_of_stations; row++) {

        int connections = get_number_of_connections(array[row]);

        for (int i = 0; i < connections; i++) {
            Connection current_connection = array[row].connections[i];
            adjacency_matrix[row][current_connection.station->id] = array[row].connections[i].route[0].duration;
            // assumes that route 0 is always to train route. If this isn't the case, the function will have to search for the correct route
        }
    }


    return *adjacency_matrix;
}

void calculate_optimal_route()
{

}



Station* debugging_data()
{
    Route *route = malloc(sizeof(Route));
    Station *stations =  malloc(5 * sizeof(Station));
    Connection *connections = malloc(2 * sizeof(Connection));


    for(int i = 0; i < 2; i++)
    {
        route->duration = i+1;
        connections[i].route = route;
        connections[i].station = &stations[i];
    }

    for(int i = 0; i < 5; i++)
    {
        stations[i].connections = connections;
    }

    for(int i = 0; i < 5; i++)
    {
        stations[i].id = i;
    }

    return stations;
}


void print_matrix(int size, int*matrix)
{
    int i, j;
    for(i=0; i<size; i++) {
        for (j = 0; j < size; j++) {
            printf("%d ", *(matrix + i * size + j)); // totally not copy-pasted code from stack overflow
        }
        printf("\n");
    }
}

