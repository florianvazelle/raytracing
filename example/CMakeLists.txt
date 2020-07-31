set(EXAMPLES
    generateSamples
)

foreach(example ${EXAMPLES})
    add_executable(${example} ${example}.cpp)
    target_link_libraries(${example} raytracing)
endforeach()
