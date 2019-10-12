set(TEST_OUTPUT_DIR ${BUILD_OUTPUT_DIR})

file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Test*.cpp)

add_executable(tests ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${TEST_SOURCES})

target_include_directories(tests
   PRIVATE
       ${CMAKE_CURRENT_SOURCE_DIR}
)

add_test(NAME tests COMMAND tests)

set_target_properties(tests PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

target_link_libraries(tests sitl Catch2::Catch2)
