add_executable(P1
        main.c
        )

add_library(UI UI.c)




target_link_libraries(P1 UI)