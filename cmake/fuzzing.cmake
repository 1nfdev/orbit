# Copyright (c) 2020 The Orbit Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

function(add_fuzzer target_name)
  # Fuzzing is (currently) only fully supported on clang. So we will
  # only enable the fuzzing-flag on clang, but we will still compile it
  # in any case, to protect the fuzz-tests from breaking. Since the
  # executable's main-function is provided by libfuzzer we have to compile
  # the code to a static library on other compilers instead.
  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_executable(${target_name} ${ARGN})
    target_compile_options(${target_name} PRIVATE ${STRICT_COMPILE_FLAGS})

    target_compile_options(${target_name} PUBLIC "-fsanitize=fuzzer")
    set_property(
      TARGET ${target_name}
      APPEND
      PROPERTY LINK_OPTIONS "-fsanitize=fuzzer")
  else()
    add_library(${target_name} STATIC ${ARGN})
    target_compile_options(${target_name} PRIVATE ${STRICT_COMPILE_FLAGS})
  endif()
endfunction()

# Usage example:
# add_fuzzer(ClassNameMethodNameFuzzer ClassNameMethodNameFuzzer.cpp)
# target_link_libraries(ClassNameMethodNameFuzzer ${PROJECT_NAME})
