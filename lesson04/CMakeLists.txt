project(lesson04)

set(lesson04_SRCS
    main.cpp
)

add_executable(lesson04 ${lesson04_SRCS})
target_link_libraries(lesson04 ${DFB_LIBRARIES})

