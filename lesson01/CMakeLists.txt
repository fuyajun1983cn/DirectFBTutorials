project(lesson01)

set(lesson01_SRCS
    main.cpp
)

add_executable(lesson01 ${lesson01_SRCS})
target_link_libraries(lesson01 ${DFB_LIBRARIES})
