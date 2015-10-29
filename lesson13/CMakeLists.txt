project(lesson13)

set(lesson13_SRCS
    main.cpp
)

add_executable(lesson13 ${lesson13_SRCS})
target_link_libraries(lesson13 ${DFB_LIBRARIES})
