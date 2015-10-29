project(lesson07)

set(lesson07_SRCS
    main.cpp
)

add_executable(lesson07 ${lesson07_SRCS})
target_link_libraries(lesson07 ${DFB_LIBRARIES})


