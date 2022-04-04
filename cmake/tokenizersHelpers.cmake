
# tokenizers_cc_test will always create a binary named ${NAME}.
# This will also add it to ctest list as ${NAME}.
# Parameters:
# NAME: name of target (see Usage below)
# SRCS: List of source files for the binary
# DEPS: List of other libraries to be linked in to the binary targets
# COPTS: List of private compile options
# DEFINES: List of public defines
# LINKOPTS: List of link options
function(tokenizers_cc_test)
  if(NOT (BUILD_TESTING AND TOKENIZERS_BUILD_TESTING))
    return()
  endif()

  cmake_parse_arguments(TOKENIZERS_CC_TEST
    ""
    "NAME"
    "SRCS;COPTS;DEFINES;LINKOPTS;DEPS"
    ${ARGN}
    )
  set(_NAME "${TOKENIZERS_CC_TEST_NAME}")
  add_executable(${_NAME} "")
  target_sources(${_NAME} PRIVATE ${TOKENIZERS_CC_TEST_SRCS})
  target_include_directories(${_NAME}
    PUBLIC ${TOKENIZERS_COMMON_INCLUDE_DIRS}
    PRIVATE ${GMOCK_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS}
    )
  target_compile_definitions(${_NAME}
    PUBLIC
    ${TOKENIZERS_CC_TEST_DEFINES}
    )
  target_compile_options(${_NAME}
    PRIVATE ${TOKENIZERS_CC_TEST_COPTS}
    )
  target_link_libraries(${_NAME}
    PUBLIC ${TOKENIZERS_CC_TEST_DEPS}
    PRIVATE ${TOKENIZERS_CC_TEST_LINKOPTS}
    )
  target_compile_features(${_NAME} PUBLIC cxx_std_17)

  add_test(NAME ${_NAME} COMMAND ${_NAME})
endfunction()
