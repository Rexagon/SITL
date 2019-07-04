set(SITL_SRC Connection.cpp)

add_library(sitl SHARED ${SITL_SRC})

target_include_directories(sitl
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(sitl Ws2_32 Boost::system)

target_compile_definitions(sitl PRIVATE SITL_EXPORTS)

target_compile_options(sitl PRIVATE
    -O2
    -pedantic
    -W
    -Wall
    -Wconversion
    -Wctor-dtor-privacy
    -Weffc++
    -Wenum-compare
    -Werror
    -Wextra
    -Wfloat-equal
    -Wlogical-op
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wshadow
    -Wsign-conversion
    -Wstrict-overflow=2
    -Wunreachable-code
)

target_compile_features(sitl PRIVATE cxx_std_17)

set_target_properties(sitl PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${BUILD_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${BUILD_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${BUILD_OUTPUT_DIR}
)

add_custom_command(TARGET sitl POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EXTLIBS_DIR}/MinGWLibs/bin/libgcc_s_dw2-1.dll ${BUILD_OUTPUT_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EXTLIBS_DIR}/MinGWLibs/bin/libstdc++-6.dll ${BUILD_OUTPUT_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EXTLIBS_DIR}/MinGWLibs/bin/libwinpthread-1.dll ${BUILD_OUTPUT_DIR}/
)
