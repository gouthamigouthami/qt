# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\peek_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\peek_autogen.dir\\ParseCache.txt"
  "peek_autogen"
  )
endif()
