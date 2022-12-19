#include "JSON-interface.h"
#include "cjson/cJSON.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calculate.h"

#define MAXIMUM_STATION_ID_LENGTH 1000

char *read_entire_file(char *filename);
ID convert_string_to_id(char* string_id);
char *get_route_id_str(char*, char*, char* output);

//This function DOES NOT check if the JSON is valid, and will break
//down quietly (which is bad).
Station *retrieve_JSON_data(char *filename) {
    char *entire_file = read_entire_file(filename);

    cJSON *json = cJSON_Parse(entire_file);

    cJSON *station_array_json = cJSON_GetObjectItem(json, "stations");
    int station_array_length = cJSON_GetArraySize(station_array_json);

    if (station_array_length <= 0) {
        fprintf(stderr, "Error: station list is empty\n");
    }

    Station *station_array = malloc((station_array_length+1) * sizeof(Station));

    //Documentation recommends this over simple iteration, as
    //it is more efficient
    int i = 0;
    cJSON *station_json;
    cJSON_ArrayForEach(station_json, station_array_json) {
        char* name = cJSON_GetObjectItem(station_json, "name")->valuestring;
        station_array[i].name = malloc(strlen(name)+1 * sizeof(char));
        strncpy(station_array[i].name, name, strlen(name)+1);

        char* string_id = cJSON_GetObjectItem(station_json, "ID")->valuestring;

        station_array[i].id = convert_string_to_id(string_id);

        /*cJSON* connection_array_json = cJSON_GetObjectItem(station_json, "connections");
        station_array[i].connections = calloc(cJSON_GetArraySize(connection_array_json)+1, sizeof(Connection));
        cJSON* connection_json;
        cJSON_ArrayForEach(connection_json, connection_array_json){
            //FIXME: Uhh how do we reference stations that don't yet exist?
        }*/

        i++;
    }
    station_array[i] = (Station){0};

    //The first pass doesn't check connections (because the stations don't exist yet)
    //We will do that at the end

    //Put all routes into memory
    cJSON* route_array_json = cJSON_GetObjectItem(json, "connections");
    Route* route_array = malloc((cJSON_GetArraySize(route_array_json) + 1) * sizeof(Route));
    cJSON* route_json;
    i = 0;
    cJSON_ArrayForEach(route_json, route_array_json){
        Route route;
        char* routeID = route_json->string; //TODO: Name this with whatever name Lasse gives this
        char* station_string_a = malloc(sizeof(char) * (MAXIMUM_STATION_ID_LENGTH+1));
        char* station_string_b = malloc(sizeof(char) * (MAXIMUM_STATION_ID_LENGTH+1));
        sscanf(routeID, "%[^-]-%s", station_string_a, station_string_b);

        route.node1 = get_station_by_id(station_array, convert_string_to_id(station_string_a));
        route.node2 = get_station_by_id(station_array, convert_string_to_id(station_string_b));
        free(station_string_a);
        free(station_string_b);
        route.line = cJSON_GetObjectItem(route_json, "lineID")->valueint;
        route.price = cJSON_GetNumberValue(cJSON_GetObjectItem(route_json, "price"));
        route.distance = cJSON_GetObjectItem(route_json, "distance")->valueint;
        route.duration = string_to_seconds(cJSON_GetObjectItem(route_json, "duration")->valuestring);
        if(strcmp(cJSON_GetObjectItem(route_json, "type")->valuestring, "rail")==0){
            route.type = RAIL;
        } else {
            route.type = AIR;
        }

        route_array[i] = route;
        i++;
    }

    i = 0;
    cJSON_ArrayForEach(station_json, station_array_json){
        int j = 0;
        cJSON* station_connection_array_json = cJSON_GetObjectItem(station_json, "connections");
        Connection* station_connection_array = malloc((1+cJSON_GetArraySize(station_connection_array_json))*sizeof(Connection));
        cJSON* station_connection_json;
        cJSON_ArrayForEach(station_connection_json, station_connection_array_json){
            Connection connection;

            //Get Connection station
            char* destination_str = cJSON_GetObjectItem(station_connection_json, "destination")->valuestring;
            connection.station = get_station_by_id(station_array, convert_string_to_id(destination_str));

            //Get Connection timetable
            int k = 0;
            cJSON* timetable_json = cJSON_GetObjectItem(station_connection_json, "timetables");
            int* timetable = malloc((cJSON_GetArraySize(timetable_json)) * sizeof(int));
            cJSON* time_json;
            cJSON_ArrayForEach(time_json, timetable_json){
                char* time_str = time_json->valuestring;
                timetable[k] = string_to_seconds(time_str);
                k++;
            }
            connection.timetable = timetable;
            connection.timetable_length = cJSON_GetArraySize(timetable_json);

            char *beginning_str = cJSON_GetObjectItem(station_json, "ID")->valuestring;

            Route *route = malloc(sizeof(Route));

            char *route_id_str = malloc((strlen(beginning_str) + 1 + strlen(destination_str) + 1) * sizeof(char));
            get_route_id_str(beginning_str, destination_str, route_id_str);
            route_json = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "routes"),
                                             route_id_str);

            if (route_json == NULL) {
                fprintf(stderr, "Route %s was not found!\n", route_id_str);
            }
            free(route_id_str);

            struct Station *station_a = get_station_by_id(station_array, convert_string_to_id(beginning_str));
            struct Station *station_b = get_station_by_id(station_array, convert_string_to_id(destination_str));

            route->node1 = (station_a->id) < (station_b->id) ? station_a : station_b;
            route->node2 = (station_a->id) > (station_b->id) ? station_a : station_b;
            route->line = cJSON_GetObjectItem(route_json, "lineID")->valueint;
            route->price = cJSON_GetObjectItem(route_json, "price")->valuedouble;
            route->distance = cJSON_GetObjectItem(route_json, "distance")->valueint;
            route->duration = string_to_seconds(cJSON_GetObjectItem(route_json, "duration")->valuestring);
            route->type = strcmp(cJSON_GetObjectItem(route_json, "type")->valuestring, "rail") == 0 ? RAIL : AIR;

            connection.route = route;

            station_connection_array[j] = connection;
            j++;
        }
        station_connection_array[j] = (Connection) {0};
        station_array[i].connections = station_connection_array;
        i++;
    }

    cJSON_Delete(json);

    return station_array;
}

//This function might fail if the file is larger than 2GB. If we want
//to solve this, we would have to use a function other than `fseek` to
//check the length of the file (would probably be OS-specific), and we
//would have to use several `fgets`s.
//TODO: Report an error if file is larger than 2GB.
char *read_entire_file(char *filename) {
    long file_size;

    FILE *file = fopen(filename, "rb");

    if (file == NULL) {
        fprintf(stderr, "File \"%s\" cannot be accessed", filename);
    }

    //Get file size
    fseek(file, 0L, SEEK_END);
    file_size = ftell(file);
    rewind(file); //TODO: Find out if this method includes space for '\0'

    //Read the entire file into a string
    char *file_string = malloc(file_size + 1);
    //fgets(file_string, file_size, file);
    fread(file_string, sizeof(char), file_size, file);

    return file_string;
}

ID convert_string_to_id(char* string_id){
    int id = 0;
    for(int j = 0; j < strlen(string_id); j++){
        id += (ID) string_id[j]<<((j%sizeof(ID))*8); //I guess this is a way
    }
    return id;
}

char* get_route_id_str(char* a, char* b, char* output){
    //char* output = malloc(strlen(a)+1+strlen(b)+1);
    output[0] = '\0';

    //Temporary solution
    if(strcmp(a, b)<0){
        strcat(output, a);
        strcat(output, "-");
        strcat(output, b);
    } else {
        strcat(output, b);
        strcat(output, "-");
        strcat(output, a);
    }
    return output;
    //REMEMBER TO FREE THIS LATER
}