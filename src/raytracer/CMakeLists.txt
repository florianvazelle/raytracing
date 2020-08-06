project("raytracer")

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})

target_link_libraries(${PROJECT_NAME} raytracing nanogui jsoncpp_lib ${NANOGUI_EXTRA_LIBS})


if(MSVC)
    add_dependencies(${PROJECT_NAME} raytracing)
    target_compile_options(${PROJECT_NAME} PRIVATE /GL)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -O3 -g)
endif()