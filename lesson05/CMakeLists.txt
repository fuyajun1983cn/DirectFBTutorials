project(lesson05)

set(lesson05_SRCS
    main.cpp
)

add_executable(lesson05 ${lesson05_SRCS})
target_link_libraries(lesson05 ${DFB_LIBRARIES})


