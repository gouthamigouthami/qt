# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\qt-7-filename_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\qt-7-filename_autogen.dir\\ParseCache.txt"
  "qt-7-filename_autogen"
  )
endif()
