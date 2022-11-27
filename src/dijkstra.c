//
// Created by lasse on 25-11-2022.
//

#include "dijkstra.h"
#include <stdio.h>
#include <malloc.h>

int* create_adjacency_matrix_for_dijkstra_algorithm(int number_of_stations)
{
    int (*adjacency_matrix)[number_of_stations] = malloc(sizeof(int[number_of_stations][number_of_stations]));

    // fill matrix with zeros, so stations are disconnected
    for(int j = 0; j < number_of_stations; j++) {
        for (int i = 0; i < number_of_stations; i++) {
            adjacency_matrix[i][j] = 0;
        }
    }



    return *adjacency_matrix;

}

void calculate_optimal_route()
{

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