
add_executable(${PROJECT_NAME}
    pg_main.cpp
    pg_game.cpp
    pg_boids.cpp
    pg_cone.cpp
    pg_update_frame.cpp
)

# TODO: can set up CMAKE_UNITY_BUILD to make compile faster

target_link_libraries(${PROJECT_NAME} raylib m)

if (${PLATFORM} STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()