project(lesson09)

set(lesson09_SRCS
    main.cpp
)

add_executable(lesson09 ${lesson09_SRCS})
target_link_libraries(lesson09 ${DFB_LIBRARIES})
