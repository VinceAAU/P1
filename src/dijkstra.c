/*
 * TODO: Fix debugging commands to properly test
 * TODO: If JSON checker doesn't order the route, the adjacency matrix function will have to. See line 42
 * TODO: Write function to check through routes for something
 */
#include "dijkstra.h"
#include <stdio.h>
#include "station.h"
#include <stdlib.h>
#include <time.h>

int get_number_of_connections(Station* station)
{
    int size = (sizeof(station->connections)/sizeof(Connection));
    return size;
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

        int connections = get_number_of_connections(array[row].connections);
        printf("There are %d connections for station %d\n",connections, row);

        for(int j = 0; j < 3; j++)
        {
            printf("%d\n", array[row].connections[j].station->id);
        }

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



Station* debugging_data(int size)
{
    // doesn't free of the memory it uses but hardly matters because it's for debugging only.
    Route *route = malloc(2 * sizeof(Route));
    Station *stations =  malloc(size * sizeof(Station));
    Connection *connections = malloc(3 * sizeof(Connection));

    route[0].duration = 5;
    route[1].duration = 7;

    srand(time(0));

    for(int i = 0; i < size; i++)
    {
        stations[i].connections = malloc(3 * sizeof(Connection));
    }
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            stations[i].connections[j].route = &route[rand()%2];
           stations[i].connections[j].station = &stations[rand()%size];
        }
    }


    for(int i = 0; i < size; i++)
    {
        stations[i].id = i;
    }

    for(int j = 0; j < 3; j++)
    {
        printf("%d\n", stations[0].connections[j].station->id);
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

