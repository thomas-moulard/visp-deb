project(tutorial-tracking-mb-hybrid)

cmake_minimum_required(VERSION 2.6)

find_package(VISP REQUIRED)
if(VISP_FOUND)
  include(${VISP_USE_FILE})
endif(VISP_FOUND)

# build the examples
add_executable(tutorial-mb-hybrid-tracker tutorial-mb-hybrid-tracker.cpp)

# copy the data
get_target_property(target_location tutorial-mb-hybrid-tracker LOCATION)
get_filename_component(target_location "${target_location}" PATH)
list(APPEND data2copy "${CMAKE_CURRENT_SOURCE_DIR}/teabox.pgm" )
list(APPEND data2copy "${CMAKE_CURRENT_SOURCE_DIR}/teabox.xml" )
list(APPEND data2copy "${CMAKE_CURRENT_SOURCE_DIR}/teabox.cao" )
list(APPEND data2copy "${CMAKE_CURRENT_SOURCE_DIR}/teabox.init" )
foreach(data ${data2copy})
  add_custom_command(
    TARGET tutorial-mb-hybrid-tracker
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${data}" "${target_location}"
  )
endforeach()
