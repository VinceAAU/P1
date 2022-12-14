
#include "dijkstra.h"
#include <stdio.h>
#include "station.h"
#include <stdlib.h>
#include <time.h>

#define INFINITY 999999 // must be higher than all route durations combined.

/* TODO: Account for route changes by checking timetables
 * if a route change is required, check the timetable and add
 * the time difference to the cost matrix
 *
 * TODO: Include airplane routes
 *make a seperate cost matrix, which includes planes
 *
 *
 */

Station * index_station_array(int number_of_stations, Station* station_array)
{
    Station *array = (Station *) malloc(number_of_stations * sizeof(Station));

    // copies in elements from first array to second, cause apparently you have to do that in C
    for (int i = 0; i < number_of_stations; i++) {
        array[i] = station_array[i];
        array[i].id = i; // changes id to array position
    }

    return array;
}

int* create_adjacency_matrix_for_dijkstra_algorithm(int number_of_stations, Station* station_array, int allow_planes_bool ) {
    int (*adjacency_matrix)[number_of_stations] = malloc(sizeof(int[number_of_stations][number_of_stations]));

    // fill matrix with zeros, so stations are disconnected by default.
    for (int j = 0; j < number_of_stations; j++) {
        for (int i = 0; i < number_of_stations; i++) {
            adjacency_matrix[i][j] = 0;
        }
    }

    // allocates for array of the same size
    Station* indexed_array = index_station_array(number_of_stations, station_array);

    for (int row = 0; row < number_of_stations; row++) {

        size_t connections = connection_list_length(indexed_array[row].connections); // TODO: replace with connection_list_length from station.h

            for (int i = 0; i < connections; i++) {
                Connection current_connection = indexed_array[row].connections[i];
                if(current_connection.route->type == RAIL) {
                    adjacency_matrix[row][current_connection.station->id] = indexed_array[row].connections[i].route->duration;
                }
                // If JSON checker doesn't order the route, the adjacency matrix function will have to.
            }
            if(allow_planes_bool)
            {
                for (int i = 0; i < connections; i++) {
                    Connection current_connection = indexed_array[row].connections[i];
                    if(current_connection.route->type == AIR)
                    {
                        adjacency_matrix[row][current_connection.station->id] = indexed_array[row].connections[i].route[1].duration;
                    }
                    // If JSON checker doesn't order the route, the adjacency matrix function will have to.
                }
            }

    }

    return *adjacency_matrix;
}

void reverse_array(Station array[], int length)
{
    Station temp[length];

    for (int i = 0; i < length; i++) {
        temp[length - 1 - i] = array[i];
    }

    for (int i = 0; i < length; i++) {
        array[i] = temp[i];
    }
}

int check_table(int current_time, Station station)
{

}

Station* calculate_optimal_route(int* G, int startnode,int endnode, int number_of_stations, Station* station_array, int* route_length, int current_time)
{
    Station* indexed_array = index_station_array(number_of_stations, station_array);
    int cost[number_of_stations][number_of_stations], distance[number_of_stations], predecessor[number_of_stations];
    int visited[number_of_stations], count, min_distance, next_node, i, j;

    // *(G + i * number_of_stations + j) is the same as G[i][j]

    /* TODO:
     * put this part in create adjacency matrix instead
     * it is currently here for debugging purposes */
    for (i = 0; i < number_of_stations; i++)
        for (j = 0; j < number_of_stations; j++)
            if (*(G + i * number_of_stations + j) == 0)
                cost[i][j] = INFINITY;
            else
                cost[i][j] = *(G + i * number_of_stations + j);

    for (i = 0; i < number_of_stations; i++) {
        distance[i] = cost[startnode][i];
        predecessor[i] = startnode;
        visited[i] = 0;
    }
    distance[startnode] = 0;
    visited[startnode] = 1;
    count = 1;
    while (count < number_of_stations - 1) {
        min_distance = INFINITY;

        //nextnode gives the node at minimum distance
        for (i = 0; i < number_of_stations; i++)
            if (distance[i] < min_distance && !visited[i]) {
                min_distance = distance[i];
                next_node = i;
            }
        //check if a better path exists through nextnode
        visited[next_node] = 1;
        for (i = 0; i < number_of_stations; i++)
            if (!visited[i])
                if (min_distance + cost[next_node][i] < distance[i]) {
                    distance[i] = min_distance + cost[next_node][i];
                    predecessor[i] = next_node;
                }
        count++;
    }

    // must get the number of stations in result before finding the station, to allocate the right amount of space
    // leads to slightly repetitive code, but im not sure if there is a way to get around it
    j = endnode;
    int counter = 1;
    do {
        j = predecessor[j];
        counter ++;
    } while (j != startnode);

    Station* optimal_path = malloc(counter * sizeof(Station));

    j = endnode;
    optimal_path[0] = station_array[endnode];
    for(i = 0; j != startnode; i++ )
    {
        j = predecessor[j];
        optimal_path[i+1] = station_array[j];
    }



    *route_length = counter;

    printf("distance: %d\n", distance[endnode]);
    if(distance[endnode] == INFINITY)
    {
        return NULL; // returns null if there is no possible route between the two nodes
    }

    reverse_array(optimal_path,counter);

    return optimal_path;

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
      //  printf("%d\n", stations[0].connections[j].station->id);
    }

    return stations;
}

void print_matrix(int size, int*matrix)
{
    int i, j;
    for(i=0; i<size; i++) {
        for (j = 0; j < size; j++) {
            printf("%d ", *(matrix + i * size + j));
        }
        printf("\n");
    }
}


