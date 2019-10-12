# sitl target

set(SITL_SRC
    Connection.cpp
    Command.cpp
    Stuff.cpp
    commands/List.cpp
    commands/Iden.cpp)

add_library(sitl SHARED ${SITL_SRC})

set(CMAKE_DEST cmake)
set(BINARY_DEST bin)
set(LIBRARY_DEST lib)
set(INCLUDE_DEST include)

target_include_directories(sitl
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${INCLUDE_DEST}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)


target_link_libraries(sitl
    PUBLIC
        Boost::system
        ws2_32
)

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

target_compile_features(sitl PUBLIC
    cxx_std_17
    cxx_user_literals
)

set_target_properties(sitl PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${BUILD_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${BUILD_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${BUILD_OUTPUT_DIR}
)

# sitl installation

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${INCLUDE_DEST}
        COMPONENT sitl)

install(TARGETS sitl EXPORT lib
        RUNTIME DESTINATION ${BINARY_DEST} COMPONENT sitl
        ARCHIVE DESTINATION ${LIBRARY_DEST} COMPONENT sitl)

install(EXPORT lib NAMESPACE sitl:: DESTINATION ${CMAKE_DEST}
        COMPONENT sitl)

install(FILES ${CMAKE_SOURCE_DIR}/cmake/sitl-config.cmake DESTINATION "./"
        COMPONENT sitl)


if (BUILD_PYSITL)
    # pysitl target

    add_library(pysitl MODULE sitl_wrap.cpp)

    target_link_libraries(pysitl
        sitl
        Python2::Python
        Boost::python27
    )

    set_target_properties(pysitl
        PROPERTIES
        PREFIX ""
        SUFFIX ".pyd"
        LIBRARY_OUTPUT_DIRECTORY ${BUILD_OUTPUT_DIR}
    )

    add_custom_command(TARGET sitl POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EXTLIBS_DIR}/MinGWLibs/bin/libgcc_s_dw2-1.dll ${BUILD_OUTPUT_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EXTLIBS_DIR}/MinGWLibs/bin/libstdc++-6.dll ${BUILD_OUTPUT_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EXTLIBS_DIR}/MinGWLibs/bin/libwinpthread-1.dll ${BUILD_OUTPUT_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Boost_PYTHON27_LIBRARY} ${BUILD_OUTPUT_DIR}
    )

    # pysitl installation

    install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${INCLUDE_DEST}
            COMPONENT pysitl EXCLUDE_FROM_ALL)

    install(TARGETS pysitl EXPORT pylib LIBRARY DESTINATION ${BINARY_DEST}
            COMPONENT pysitl)

    install(EXPORT pylib NAMESPACE sitl:: DESTINATION ${CMAKE_DEST}
            COMPONENT pysitl)

    install(FILES ${CMAKE_SOURCE_DIR}/cmake/sitl-config.cmake DESTINATION "./"
            COMPONENT pysitl EXCLUDE_FROM_ALL)
endif()
