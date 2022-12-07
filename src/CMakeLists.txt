

add_library(UI UI.c)
add_library(dijkstra dijkstra.c)
add_library(station station.c)
add_library(calculate calculate.c)
add_library(JSON-interface JSON-interface.c)


add_executable(P1 main.c)


target_link_libraries(P1 UI dijkstra station calculate JSON-interface)