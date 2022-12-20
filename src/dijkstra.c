
#include "dijkstra.h"
#include <stdio.h>
#include "station.h"
#include <stdlib.h>
#include <time.h>

#define INFINITY 999999 // must be higher than all route durations combined.

/*
 * TODO: Account for route changes by checking timetables
 *
* if a route change is required, check the timetable and add
 * the time difference to the cost matrix
 * TODO: Actually free memory lol
 */

/**
 * Indexes array so a stations ID represents its position in the array. This allows makes it easier to make a matrix for the connections.
 * @param number_of_stations
 * @param station_array array to be indexed
 * @note This functions doesn't create new pointers for connections and routes, so attempting to free it will probably free some of the original array aswell.
 * @return pointer to indexed array
 */
Station * index_station_array(size_t number_of_stations, Station* station_array)
{
    Station *array = (Station *) malloc(number_of_stations * sizeof(Station));

    // copies in elements from first array to second, cause apparently you have to do that in C
    for (int i = 0; i < number_of_stations; i++) {
        array[i] = station_array[i];
       // printf("Station: %d",array[i].id);
        array[i].id = i; // changes id to array position
       // printf(" is now %d\n", array[i].id);
    }


    return array;
}
/**
 * Indexes array so a stations ID represents its position in the array. This allows makes it easier to make a matrix for the connections.
 * @param number_of_stations
 * @param station_array array to create matrix from. Must be indexed!
 * @param allow_planes_bool
 * @note station_array must be indexed with index_station_array. Will cause segfault if it receives faulty data.
 * @return pointer to adjacency matrix
 */
int* create_adjacency_matrix_for_dijkstra_algorithm(size_t number_of_stations, Station* station_array, int allow_planes_bool ) {
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

        size_t connections = connection_list_length(indexed_array[row].connections);

            for (int i = 0; i < connections; i++) {
                Connection current_connection = indexed_array[row].connections[i];
            //    printf("%d\n",indexed_array[row].id);
                if(current_connection.route->type == RAIL) {
                    adjacency_matrix[row][current_connection.station->id-65] = indexed_array[row].connections[i].route->duration;
                    //printf("\n Setting:%d, %d to %d ",row, current_connection.station->id-65,indexed_array[row].connections[i].route->duration);
                }
                // -65 on the connection IDS is a hotfix. This wouldn't work if stations had ids beyond one letter
            }
            if(allow_planes_bool)
            {
                for (int i = 0; i < connections; i++) {
                    Connection current_connection = indexed_array[row].connections[i];
                    if(current_connection.route->type == AIR)
                    {
                        adjacency_matrix[row][current_connection.station->id-65] = indexed_array[row].connections[i].route->duration+7200; // 2 hours added for signing in/ going through security in airports
                    }
                }
            }

    }

  //  print_matrix(23,*adjacency_matrix);
    return *adjacency_matrix;
}
/**
 * Reverses the order of an array. This is a simple fix to calculate_optimal_route returning station list in the wrong order
 * @param array array to be reversed
 * @param length number of elements in array
 */
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


/**
 * @param adjacency_matrix adjacency matrix, must be the same size as number_of_stations^2
 * @param startnode start station
 * @param endnode end station
 * @param number_of_stations total number of stations
 * @param station_array all stations available to travel to/from
 * @param current_time when the user begins their travel
 * @param output_time output parameter, which gives the total travel time
 * @return Array of stations in the optimal route. Length of the array depends on the length of the route.
 * returns null if there is no possible route between startnode and endnode
 * @note train travel is still used when airtravel is allowed. This can lead to the function returning only train routes despite using the AIR type.
 */
Station* calculate_optimal_route(int* adjacency_matrix, int startnode,int endnode, size_t number_of_stations, Station* station_array, int current_time, int *output_time)
{
    Station* indexed_array = index_station_array(number_of_stations, station_array);
    int cost[number_of_stations][number_of_stations], distance[number_of_stations], predecessor[number_of_stations];
    int visited[number_of_stations], count, min_distance, next_node, i, j;

    startnode -=65;
    endnode -=65;
    // -65 on the nodes is a hotfix. This wouldn't work if stations had ids beyond one letter
    // *(adjacency_matrix + i * number_of_stations + j) is the same as G[i][j] in 2d arrays, but that syntax isn't allowed here


    // sets all the zeros i.e
    for (i = 0; i < number_of_stations; i++)
        for (j = 0; j < number_of_stations; j++)
            if (*(adjacency_matrix + i * number_of_stations + j) == 0)
                cost[i][j] = INFINITY;
            else
                cost[i][j] = *(adjacency_matrix + i * number_of_stations + j);

  //  print_matrix(23,*cost);
 //   printf("startNode %d",startnode);

    for (i = 0; i < number_of_stations; i++) {
  //      printf("%d\n",cost[startnode][i]);
        distance[i] = cost[startnode][i];
        predecessor[i] = startnode;
        visited[i] = 0;
    }
    distance[startnode] = 0;
    visited[startnode] = 1;
    count = 1;
  //  printf("\n%d\n", next_node);
    while (count < number_of_stations - 1) {
        min_distance = INFINITY;

        //nextnode gives the node at minimum distance
        for (i = 0; i < number_of_stations -1; i++) {
         //   printf("\nDist: %d\n",distance[i]);
       //     printf("\nVisit: %d\n", visited[i]);
            if (distance[i] < min_distance && !visited[i]) {
                min_distance = distance[i];
                next_node = i;
              //  printf("\nCondition true!%d\n", next_node);
            }
           // printf("\n%d\n", next_node);
        }

        //printf("\n%d\n", next_node);
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

    Station *optimal_path = malloc((counter + 1) * sizeof(Station));

    optimal_path[counter] = (Station){0};

    j = endnode;
    optimal_path[0] = station_array[endnode];
    for(i = 0; j != startnode; i++ )
    {
        j = predecessor[j];
        optimal_path[i+1] = station_array[j];
    }

    *output_time = distance[endnode];
    if(distance[endnode] == INFINITY)
    {
        return NULL; // returns null if there is no possible route between the two nodes
    }

    // reverses array
    reverse_array(optimal_path,counter);


    return optimal_path;

}
/**
 * Creates some random data used debugging before the actual data is available
 * @param size size of the array of stations
 * @return array of stations
 */
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

/**
 * Prints an adjacency matrix. Used for debugging
 * @param size amount of stations in the matrix
 * @param matrix pointer to an adjacency matrix
 */
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

/**Simply combines create_adjacency_matrix with calculate_optimal_path so you don't have to in main
 * @param number_of_stations total number of stations
 * @param station_array all stations available to travel to/from
 * @param travel_type determines whether dijkstra's will allow travel by air
 * @param current_time when the user begins their travel
 * @param output_time output parameter, which gives the total travel time
 * @param start_node start station ID
 * @param end_node end station ID
 * @return Array of stations in the optimal route. Length of the array depends on the length of the route.
 * returns null if there is no possible route between startnode and endnode
 * @note train travel is still used when airtravel is allowed. This can lead to the function returning only train routes despite using the AIR type.
 */
Station* run_dijkstras(size_t total_number_of_stations, Station* all_stations, TravelType travel_type, int current_time, int* output_time, ID start_node, ID end_node)
{
    int * matrixrail;
    int * matrixair;
    Station * optimal_route;
    if(travel_type == AIR) {
        matrixair = create_adjacency_matrix_for_dijkstra_algorithm(total_number_of_stations, all_stations, 1);
        optimal_route = calculate_optimal_route(matrixair, start_node, end_node, total_number_of_stations, all_stations, current_time, output_time);
    }
    else {
        matrixrail = create_adjacency_matrix_for_dijkstra_algorithm(total_number_of_stations, all_stations, 0);
        optimal_route = calculate_optimal_route(matrixrail, start_node, end_node, total_number_of_stations, all_stations, current_time, output_time);
    }

    return optimal_route;
}

