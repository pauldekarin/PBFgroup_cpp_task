# Common CMakeLists for shared code
set(COMMON_SRC_DIR ${CMAKE_SOURCE_DIR}/common/src)

# Collect all common source files
file(GLOB COMMON_SOURCES
    "${COMMON_SRC_DIR}/*.cpp"
)

# Create a library from the common files
add_library(common STATIC ${COMMON_SOURCES})

# Add include directory for common headers
target_include_directories(common PUBLIC ${CMAKE_SOURCE_DIR}/common)

# Specify the output directory for the common library
set_target_properties(common PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
