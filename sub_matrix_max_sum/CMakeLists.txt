# CMakeList.txt : CMake project for sub_matrix_max_sum, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (sub_matrix_max_sum "sub_matrix_max_sum.cpp" "sub_matrix_max_sum.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET sub_matrix_max_sum PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
