cmake_minimum_required(VERSION 2.6)
project(sl C)
enable_testing()

if(NOT CMAKE_COMPILER_IS_GNUCC)
  message(FATAL_ERROR "Unsupported compiler")
endif(NOT CMAKE_COMPILER_IS_GNUCC)

set(CMAKE_DEBUG_POSTFIX "-dbg")

set(CMAKE_C_FLAGS "-pedantic -std=c99 -Wall -Wextra -Wcast-align -Wmissing-declarations -Wmissing-prototypes -fvisibility=hidden -fstrict-aliasing -Wl,-z,defs")
set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

################################################################################
# Check dependencies
################################################################################
find_path(SNLSYS_INCLUDE_DIR "snlsys/snlsys.h")
find_library(SNLSYS_LIBRARY snlsys)

if(NOT SNLSYS_INCLUDE_DIR OR NOT SNLSYS_LIBRARY)
  message(FATAL_ERROR "Can't find snlsys header/library")
else()
  message(STATUS "snlsys found: ${SNLSYS_LIBRARY}")
endif()

include_directories(${SNLSYS_INCLUDE_DIR})

################################################################################
# Define targets
################################################################################
file(GLOB SL_FILES_SRC *.c)
file(GLOB SL_FILES_INC *.h *.h.def)

add_library(sl SHARED ${SL_FILES_SRC} ${SL_FILES_INC})
target_link_libraries(sl ${SNLSYS_LIBRARY})
set_target_properties(sl PROPERTIES DEFINE_SYMBOL SL_SHARED_BUILD)

################################################################################
# Add tests
################################################################################
macro(add_sl_test suffix)
  add_executable(test_sl_${suffix} test/test_sl_${suffix}.c)
  target_link_libraries(test_sl_${suffix} sl)
  add_test(test_sl_${suffix} test_sl_${suffix})
endmacro()

add_sl_test(flat_map)
add_sl_test(flat_set)
add_sl_test(hash_table)
add_sl_test(logger)
add_sl_test(string)
add_sl_test(vector)

################################################################################
# Define output & install directories
################################################################################
install(TARGETS sl LIBRARY DESTINATION lib)
install(FILES ${SL_FILES_INC} DESTINATION include/sl)

