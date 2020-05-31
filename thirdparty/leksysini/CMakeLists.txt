CMAKE_MINIMUM_REQUIRED (VERSION 2.6 FATAL_ERROR)
PROJECT(TEST_INI_APP)
# Test application
ADD_EXECUTABLE(test_ini_app iniparser.hpp test_app.cpp)