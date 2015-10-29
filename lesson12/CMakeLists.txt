project(lesson12)

set(lesson12_SRCS
    main.cpp
)

add_executable(lesson12 ${lesson12_SRCS})
target_link_libraries(lesson12 ${DFB_LIBRARIES})
