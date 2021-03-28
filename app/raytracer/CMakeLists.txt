project("raytracer")

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)

file(GLOB_RECURSE ${PROJECT_NAME}_COMMON_HEADERS ${PROJECT_SOURCE_DIR}/../common/*.h)
file(GLOB_RECURSE ${PROJECT_NAME}_COMMON_SOURCES ${PROJECT_SOURCE_DIR}/../common/*.cpp)

if(MSVC)
  file(GLOB GLAD_SOURCE ${PROJECT_SOURCE_DIR}/../external/nanogui/ext/glad/src/glad.c)

  add_executable(
    ${PROJECT_NAME}
    ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_COMMON_HEADERS} ${${PROJECT_NAME}_COMMON_SOURCES}
    ${PROJECT_SOURCE_DIR}/../../external/nanogui/ext/glad/src/glad.c)
    
    target_compile_options(${PROJECT_NAME} PRIVATE /GL)
else(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  add_executable(
    ${PROJECT_NAME}
    ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_COMMON_HEADERS} ${${PROJECT_NAME}_COMMON_SOURCES})

  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -O3 -g)
endif()

target_link_libraries(${PROJECT_NAME} raytracing nanogui jsoncpp_lib ${NANOGUI_EXTRA_LIBS})