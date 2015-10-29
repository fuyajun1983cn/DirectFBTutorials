project(lesson10)

set(lesson10_SRCS
    main.cpp
)

add_executable(lesson10 ${lesson10_SRCS})
target_link_libraries(lesson10 ${DFB_LIBRARIES})
