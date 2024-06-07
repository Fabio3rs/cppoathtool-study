add_executable(cppoathtool_tests test.cpp)
target_link_libraries(cppoathtool_tests cppoathtool_lib GTest::GTest GTest::Main)
gtest_discover_tests(cppoathtool_tests)
