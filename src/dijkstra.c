//
// Created by lasse on 25-11-2022.
//

#include "dijkstra.h"
#include <stdio.h>
#include <malloc.h>
#include "station.h"

/** Might not be necessary if the JSON checker saves the length of the array somehow
 *
 * @param station_array
 * @return number of elements
 */
int get_number_of_stations(Station* station_array)
{
    // how many bytes the array is divided by how many bytes one element is gives you the number of elements
    return (sizeof(&station_array)/sizeof(Station));
}
/**
 *
 * @param number_of_stations
 * @param station_array
 * @return adjacency matrix... duuuh
 */
int* create_adjacency_matrix_for_dijkstra_algorithm(int number_of_stations, Station* station_array )
{
    int (*adjacency_matrix)[number_of_stations] = malloc(sizeof(int[number_of_stations][number_of_stations]));

    // fill matrix with zeros, so stations are disconnected by default.
    for(int j = 0; j < number_of_stations; j++) {
        for (int i = 0; i < number_of_stations; i++) {
            adjacency_matrix[i][j] = 0;
        }
    }

    /* Might be a silly solution, but to create an adjacency matrix I need to know where the station
     * from each connection is located in the array of stations. This duplicates the array and
     * changes the ids to represent where the station is in the array, so I can jump to that station's
     * row/column in the matrix and change the value appropriately. */

    // allocates for array of the same size
    Station *array = (Station *) malloc(number_of_stations * sizeof(Station));

    // copies in elements from first array to second, cause apparently you have to do that in C
    for(int i = 0; i < number_of_stations; i++)
    {
        array[i] = station_array[i];
        array[i].id = i; // changes id to array position
    }



    // for every connection in array, check travel time
    // find the location of the connected station in the array somehow? use search algorithm or change how station ids work.


    return *adjacency_matrix;
}

/**
 * very descriptive description
 */
void calculate_optimal_route()
{

}

/**
 * @param size
 * @param matrix
 */
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