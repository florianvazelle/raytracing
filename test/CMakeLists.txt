file(GLOB_RECURSE ${PROJECT_NAME}_TESTS ${PROJECT_SOURCE_DIR}/test/Test*.cpp)

add_executable(rtx_test ${${PROJECT_NAME}_TESTS})

target_link_libraries(rtx_test raytracing gtest)
