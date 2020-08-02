file(GLOB_RECURSE ${PROJECT_NAME}_TESTS ${PROJECT_SOURCE_DIR}/test/Test*.cpp)

add_executable(rtx_test ${${PROJECT_NAME}_TESTS})

target_link_libraries(rtx_test raytracing gtest)

# Run unit tests in post-build
if(RTX_WITH_POST_BUILD_UNITTEST)
    add_custom_command(TARGET rtx_test
        POST_BUILD
        COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:rtx_test>
    )
endif()