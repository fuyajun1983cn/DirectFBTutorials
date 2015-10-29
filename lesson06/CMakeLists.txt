project(lesson06)

set(lesson06_SRCS
    main.cpp
)

add_executable(lesson06 ${lesson06_SRCS})
target_link_libraries(lesson06 ${DFB_LIBRARIES})

