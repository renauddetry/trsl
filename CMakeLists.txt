PROJECT(TemplateRangeSamplingLibrary)

SET(PROJECT_NAMESPACE trsl)
SET(CMAKE_BUILD_TYPE RelWithDebInfo)

 
# Headers and implementation files for the library will
# be found in a directory named ${PROJECT_NAMESPACE}
FILE(GLOB HEADERS ${PROJECT_NAMESPACE}/*.hpp)

ADD_EXECUTABLE(${PROJECT_NAMESPACE}_example1
               examples/${PROJECT_NAMESPACE}_example1.cpp)
ADD_EXECUTABLE(${PROJECT_NAMESPACE}_example1+
               examples/${PROJECT_NAMESPACE}_example1+.cpp)
ADD_EXECUTABLE(${PROJECT_NAMESPACE}_example2
               examples/${PROJECT_NAMESPACE}_example2.cpp)
ADD_EXECUTABLE(test_is_picked_systematic
               tests/test_is_picked_systematic.cpp)
ADD_EXECUTABLE(accessor_efficiency
               tests/accessor_efficiency.cpp tests/accessor_no_inline.cpp)


INCLUDE_DIRECTORIES(.)

ADD_DEFINITIONS(-Wall)

INSTALL(FILES ${HEADERS} DESTINATION include/${PROJECT_NAMESPACE})