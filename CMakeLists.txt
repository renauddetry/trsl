PROJECT(TemplateRangeSamplingLibrary)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

SET(PROJECT_NAMESPACE trsl)
SET(CMAKE_BUILD_TYPE RelWithDebInfo)

 
# Headers and implementation files for the library will
# be found in a directory named ${PROJECT_NAMESPACE}
FILE(GLOB HEADERS ${PROJECT_NAMESPACE}/*.hpp)

ADD_EXECUTABLE(${PROJECT_NAMESPACE}_example1
               examples/${PROJECT_NAMESPACE}_example1.cpp)
ADD_EXECUTABLE(${PROJECT_NAMESPACE}_example1plus
               examples/${PROJECT_NAMESPACE}_example1plus.cpp)
ADD_EXECUTABLE(${PROJECT_NAMESPACE}_example2
               examples/${PROJECT_NAMESPACE}_example2.cpp)
ADD_EXECUTABLE(test_is_picked_systematic
               tests/test_is_picked_systematic.cpp)
ADD_EXECUTABLE(test_systematic_sample_iterator
               tests/test_systematic_sample_iterator)
ADD_EXECUTABLE(test_random_reord_iter
               tests/test_random_reord_iter.cpp)
ADD_EXECUTABLE(test_sorted_reord_iter
               tests/test_sorted_reord_iter.cpp)
ADD_EXECUTABLE(test_custom_reord_iter
               tests/test_custom_reord_iter.cpp)
ADD_EXECUTABLE(accessor_efficiency
               tests/accessor_efficiency.cpp tests/accessor_no_inline.cpp)
ADD_EXECUTABLE(reorder_iterator_efficiency
               tests/reorder_iterator_efficiency.cpp)
ADD_EXECUTABLE(test_boost_typeof
               tests/test_boost_typeof.cpp)


INCLUDE_DIRECTORIES(.)

STRING(REGEX REPLACE "-I([^ ]+)" "\\1" CPPFLAGS "$ENV{CPPFLAGS}")
SEPARATE_ARGUMENTS(CPPFLAGS)
INCLUDE_DIRECTORIES(${CPPFLAGS})

ADD_DEFINITIONS(-Wall)

INSTALL(FILES ${HEADERS} DESTINATION include/${PROJECT_NAMESPACE})
