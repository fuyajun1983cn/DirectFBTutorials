project(lesson03)

set(lesson03_SRCS
    main.cpp
)

add_executable(lesson03 ${lesson03_SRCS})
target_link_libraries(lesson03 ${DFB_LIBRARIES})


