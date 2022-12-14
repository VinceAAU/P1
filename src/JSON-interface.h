#pragma once

#include <stdio.h>

#include "station.h"

/**
 * Converts JSON data to Station array
 * @param filename The JSON file's filename
 * @return A zero-terminated station array.
 */
Station* retrieve_JSON_data(char* filename);
