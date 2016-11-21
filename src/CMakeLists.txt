add_library (Vector STATIC vector.hpp)
set_target_properties(Vector PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories (Vector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
