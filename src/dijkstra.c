
#include "dijkstra.h"
#include <stdio.h>
#include "station.h"
#include <stdlib.h>
#include <time.h>

#define INFINITY 999999 // must be higher than all route durations combined.

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

        int connections = 3; // TODO: replace with connection_list_length from station.h
       // printf("There are %d connections for station %d\n",connections, row);

        for(int j = 0; j < connections; j++)
        {
          //  printf("%d\n", array[row].connections[j].station->id);
        }

        for (int i = 0; i < connections; i++) {
            Connection current_connection = array[row].connections[i];
            adjacency_matrix[row][current_connection.station->id] = array[row].connections[i].route[0].duration;
            // If JSON checker doesn't order the route, the adjacency matrix function will have to.
        }
    }


    return *adjacency_matrix;
}

void calculate_optimal_route(int* G, int startnode, int number_of_stations)
{
    int cost[number_of_stations][number_of_stations], distance[number_of_stations], pred[number_of_stations];
    int visited[number_of_stations], count, mindistance, nextnode, i, j;
    //pred[] stores the predecessor of each node
    //count gives the number of nodes seen so far
    //create the cost matrix
    // *(G + i * number_of_stations + j) is the same as G[i][j]
    for (i = 0; i < number_of_stations; i++)
        for (j = 0; j < number_of_stations; j++)
            if (*(G + i * number_of_stations + j) == 0)
                cost[i][j] = INFINITY;
            else
                cost[i][j] = *(G + i * number_of_stations + j);
    //initialize pred[],distance[] and visited[]
    for (i = 0; i < number_of_stations; i++) {
        distance[i] = cost[startnode][i];
        pred[i] = startnode;
        visited[i] = 0;
    }
    distance[startnode] = 0;
    visited[startnode] = 1;
    count = 1;
    while (count < number_of_stations - 1) {
        mindistance = INFINITY;
        //nextnode gives the node at minimum distance
        for (i = 0; i < number_of_stations; i++)
            if (distance[i] < mindistance && !visited[i]) {
                mindistance = distance[i];
                nextnode = i;
            }
        //check if a better path exists through nextnode
        visited[nextnode] = 1;
        for (i = 0; i < number_of_stations; i++)
            if (!visited[i])
                if (mindistance + cost[nextnode][i] < distance[i]) {
                    distance[i] = mindistance + cost[nextnode][i];
                    pred[i] = nextnode;
                }
        count++;
    }

    //print the path and distance of each node
    for (i = 0; i < number_of_stations; i++)
        if (i != startnode) {
            printf("\nDistance of node %d = %d", i, distance[i]);
            printf("\nPath = %d", i);
            j = i;
            do {
                j = pred[j];
                printf("<-%d", j);
            } while (j != startnode);
        }
}


Station* debugging_data(int size)
{
    // doesn't free of the memory it uses but hardly matters because it's for debugging only.
    Route *route = malloc(2 * sizeof(Route));
    Station *stations =  malloc(size * sizeof(Station));

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
     //   printf("%d\n", stations[0].connections[j].station->id);
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

