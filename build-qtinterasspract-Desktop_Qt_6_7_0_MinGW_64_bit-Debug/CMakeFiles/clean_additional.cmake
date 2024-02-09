# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\qtinterasspract_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\qtinterasspract_autogen.dir\\ParseCache.txt"
  "qtinterasspract_autogen"
  )
endif()
