project(lesson08)

set(lesson08_SRCS
    main.cpp
)

add_executable(lesson08 ${lesson08_SRCS})
target_link_libraries(lesson08 ${DFB_LIBRARIES})


