cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# option(BUILD_TESTING "build the test" ON)
option(TOKENIZERS_BUILD_TESTING OFF)

project(tokenizers
    VERSION 0.1.0
    LANGUAGES CXX)

include(CTest)
set(CTEST_TEST_TIMEOUT 1)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "build type: ${CMAKE_BUILD_TYPE}")

if (WIN32)
    add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES)
endif()

set(tokenizers_third_party_dir ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

# Adhere to GNU filesystem layout conventions
include(GNUInstallDirs)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" CACHE PATH "Archive output dir.")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" CACHE PATH "Library output dir.")
set(CMAKE_PDB_OUTPUT_DIRECTORY     "${CMAKE_BINARY_DIR}/bin" CACHE PATH "PDB (MSVC debug symbol)output dir.")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" CACHE PATH "Executable/dll output dir.")

# when tokenizers is included as subproject (i.e. using add_subdirectory(tokenizers))
# in the source tree of a project that uses it, install rules are disabled.
if(NOT CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    option(TOKENIZERS_ENABLE_INSTALL "Enable install rule" OFF)
else()
    option(TOKENIZERS_ENABLE_INSTALL "Enable install rule" ON)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(tokenizersHelpers)

# ICU Components Reference:
# https://cmake.org/cmake/help/latest/module/FindICU.html
# ICU components = data, i18n, io, le, lx, test, tu and uc.
find_package(ICU 66.0 COMPONENTS uc io i18n REQUIRED)

set(TOKENIZERS_COMMON_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/tokenizers/lib
    ${CMAKE_CURRENT_SOURCE_DIR})

if(BUILD_TESTING AND TOKENIZERS_BUILD_TESTING)
    add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/googletest third_party/googletest)
    # When Google Test is included directly rather than through find_package, the aliases are missing.
    add_library(GTest::gtest ALIAS gtest)
    add_library(GTest::gtest_main ALIAS gtest_main)
    add_library(GTest::gmock ALIAS gmock)
    add_library(GTest::gmock_main ALIAS gmock_main)
    mark_as_advanced(
        BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
        gmock_build_tests gtest_build_samples gtest_build_tests
        gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
        )
    set_target_properties(gtest PROPERTIES FOLDER third_party)
    set_target_properties(gtest_main PROPERTIES FOLDER third_party)
    set_target_properties(gmock PROPERTIES FOLDER third_party)
    set_target_properties(gmock_main PROPERTIES FOLDER third_party)

    set(DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")
    configure_file(data.h.in data.h @ONLY)
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
    list(APPEND TOKENIZERS_COMMON_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR})
    enable_testing()
    include(GoogleTest)
endif()

include_directories(${TOKENIZERS_COMMON_INCLUDE_DIRS})
add_subdirectory(tokenizers)

if (TOKENIZERS_ENABLE_INSTALL)
    include(CMakePackageConfigHelpers)
    # export & install targets
    install(
        TARGETS tokenizers
        bert_tokenizer
        fundamental_tokenizer
        basic_tokenizers
        utils
        unilib
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION lib
        # ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )
    install(
        EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

    set (TOKENIZERS_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include" )
    configure_package_config_file (
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/tokenizersConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/tokenizersConfig.cmake
        PATH_VARS TOKENIZERS_INCLUDE_DIR
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        # NO_CHECK_REQUIRED_COMPONENTS_MACRO # Eigen does not provide components
        )
    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        VERSION ${RANKPIPE_BUILD_VERSION}
        COMPATIBILITY AnyNewerVersion
        # COMPATIBILITY SameMajorVersion
        )
    install ( FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} )

    # filter & install header files
    INSTALL (
        DIRECTORY ${PROJECT_SOURCE_DIR}/tokenizers
        DESTINATION include
        FILES_MATCHING PATTERN "*.h*")
endif() # TOKENIZERS_ENABLE_INSTALL
