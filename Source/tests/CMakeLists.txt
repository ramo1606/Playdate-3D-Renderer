cmake_minimum_required(VERSION 3.14)

# Set the project name
project(3D_renderer_tests)

# Add the test executable
add_executable(run_tests
    test_display.c
    #test_paddle.c
    #test_ball.c
    #test_brick.c
)

# Include the main project's include directory
target_include_directories(run_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/Source/include
)

# Link against the main project's library
target_link_libraries(run_tests PRIVATE 
    ${PLAYDATE_GAME_NAME}
)

# Add the minunit header
target_include_directories(run_tests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# If using a specific test framework, you might need to link against it here
# target_link_libraries(run_tests PRIVATE your_test_framework)

# Copy test assets if needed
# add_custom_command(TARGET run_tests POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/tests/test_assets $<TARGET_FILE_DIR:run_tests>/test_assets
# )

# Add a custom target to run the tests
add_custom_target(test
    COMMAND run_tests
    DEPENDS run_tests
)