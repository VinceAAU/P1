#include "station.h"
#include "dijkstra.h"
#include <stdio.h>
int main(void) {

    int size = 5;

    Station* data = debugging_data(size);

    int *matrix = create_adjacency_matrix_for_dijkstra_algorithm(size,data);

    int route_length;

    print_matrix(size,matrix);

    int start_station=0;
    int end_station=4;

 //   printf("Please input startnode and endnode\n");

  /*scanf("%d",&start_station);
    scanf("%d",&end_station); */

    Station * optimal_route = calculate_optimal_route(matrix,start_station,end_station,size,data, &route_length);

    debug_print_stations(route_length,optimal_route);


}
