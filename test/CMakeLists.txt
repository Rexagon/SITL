set(TEST_OUTPUT_DIR ${BUILD_OUTPUT_DIR})

file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Test*.cpp)
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_SOURCE})

    target_include_directories(${TEST_NAME}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${EXTLIBS_DIR}/catch2/include
    )

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

    set_target_properties(${TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
    )

    target_link_libraries(${TEST_NAME} sitl)
endforeach()
