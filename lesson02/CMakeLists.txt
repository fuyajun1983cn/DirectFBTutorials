project(lesson02)

set(lesson02_SRCS
    main.cpp
)

add_executable(lesson02 ${lesson02_SRCS})
target_link_libraries(lesson02 ${DFB_LIBRARIES})

