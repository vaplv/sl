# Copyright (c) 2013 Vincent Forest
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
# EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
cmake_minimum_required(VERSION 2.6)
project(sl C)
enable_testing()

if(NOT CMAKE_COMPILER_IS_GNUCC)
  message(FATAL_ERROR "Unsupported compiler.")
endif(NOT CMAKE_COMPILER_IS_GNUCC)

set(CMAKE_C_FLAGS "-pedantic -std=c99 -Wall -Wextra -Wcast-align -Wmissing-declarations -Wmissing-prototypes -fvisibility=hidden -fstrict-aliasing -Wl,-z,defs")
set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

################################################################################
# Check dependencies
################################################################################
find_path(SYS_INCLUDE_DIR "sys/sys.h")
find_library(SYS_LIBRARY sys)
find_path(SYS_LIBRARY_DIR "${SYS_LIBRARY}")

message(STATUS "${SYS_LIBRARY}")

if(NOT SYS_INCLUDE_DIR OR NOT SYS_LIBRARY)
  message(FATAL_ERROR "Can't find sys header/library")
else()
  message(STATUS "sys found: ${SYS_LIBRARY}")
endif()

include_directories(${SYS_INCLUDE_DIR})

################################################################################
# Define targets
################################################################################
file(GLOB SL_FILES_SRC src/*.c)
file(GLOB SL_FILES_INC src/*.h src/*.h.def)

add_library(sl SHARED ${SL_FILES_SRC} ${SL_FILES_INC})
target_link_libraries(sl ${SYS_LIBRARY})
set_target_properties(sl PROPERTIES DEFINE_SYMBOL SL_SHARED_BUILD)

################################################################################
# Add tests
################################################################################
macro(add_sl_test suffix)
  add_executable(test_sl_${suffix} test/test_sl_${suffix}.c)
  target_link_libraries(test_sl_${suffix} sl)
  add_test(test_sl_${suffix} test_sl_${suffix})
endmacro()

include_directories(src)

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

