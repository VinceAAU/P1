#include "JSON-interface.h"
#include "cjson/cJSON.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *read_entire_file(char *filename);
ID convert_string_to_id(char* string_id);

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
    //We do that now
    i = 0;
    cJSON_ArrayForEach(station_json, station_array_json){
        int j = 0;
        cJSON* station_connection_array_json = cJSON_GetObjectItem(station_json, "connections");
        Connection* station_connection_array = malloc((1+cJSON_GetArraySize(station_connection_array_json))*sizeof(Connection));
        cJSON* station_connection_json;
        cJSON_ArrayForEach(station_connection_json, station_connection_array_json){
            Connection connection;

            char* destination_str = cJSON_GetObjectItem(station_connection_json, "destination")->valuestring;
            connection.station = get_station_by_id(station_array, convert_string_to_id(destination_str));

            int k = 0;
            cJSON* timetable_json = cJSON_GetObjectItem(station_connection_json, "timetable");
            struct tm* timetable = malloc((cJSON_GetArraySize(timetable_json)+1) * sizeof(struct tm));
            cJSON* time_json;
            cJSON_ArrayForEach(time_json, timetable_json){
                char* time_str = time_json->valuestring;
                struct tm time_structure;
                strptime(time_str, "%H:%M", &time_structure);
                timetable[k] = time_structure;
                k++;
            }
            timetable[k] = (struct tm){0};

            Route route;
            cJSON* route_json = cJSON_GetObjectItem(
                    cJSON_GetObjectItem(json, "connections"),

        }
        station_array[i].connections;
        i++;
    }

    cJSON_Delete(json);
}

//This function might fail if the file is larger than 2GB. If we want
//to solve this, we would have to use a function other than `fseek` to
//check the length of the file (would probably be OS-specific), and we
//would have to use several `fgets`s.
//TODO: Report an error if file is larger than 2GB.
char *read_entire_file(char *filename) {
    long file_size;

    FILE *file = fopen(filename, "rb");

    //Get file size
    fseek(file, 0L, SEEK_END);
    file_size = ftell(file);
    rewind(file); //TODO: Find out if this method includes space for '\0'

    //Read the entire file into a string
    char *file_string = malloc(file_size + 1);
    fgets(file_string, file_size, file);

    return file_string;
}

ID convert_string_to_id(char* string_id){
    int id = 0;
    for(int j = 0; j < strlen(string_id); j++){
        id += (ID) string_id[j]<<((j%sizeof(ID))*8); //I guess this is a way
    }
    return id;
}