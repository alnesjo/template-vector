add_executable (VectorTest vector.cpp)
target_link_libraries (VectorTest LINK_PUBLIC ${GTEST_LIBRARIES} pthread Vector)
