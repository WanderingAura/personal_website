
add_executable(${PROJECT_NAME}
    pg_main.cpp
    pg_game.cpp
    pg_boids.cpp
    pg_boids_display.cpp
    pg_cone.cpp
    pg_update_frame.cpp
    pg_tooltip.cpp
    pg_hoverable_box.cpp
    pg_crosshair.cpp
    pg_room.cpp
    pg_lighting.cpp
)

target_link_libraries(${PROJECT_NAME} raylib m)

if (${PLATFORM} STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
else()
    # desktop builds load assets from a relative "assets/" path next to the executable
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    )
endif()