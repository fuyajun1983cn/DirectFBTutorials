project(lesson11)

set(lesson11_SRCS
    main.cpp
)

add_executable(lesson11 ${lesson11_SRCS})
target_link_libraries(lesson11 ${DFB_LIBRARIES})
