cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(tokenizers
    VERSION 0.1.0
    LANGUAGES CXX)

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

# ICU Components Reference:
# https://cmake.org/cmake/help/latest/module/FindICU.html
# ICU components = data, i18n, io, le, lx, test, tu and uc.
find_package(ICU 66.0 COMPONENTS uc io i18n REQUIRED)
# find_package(ICU COMPONENTS data uc i18n io le lx REQUIRED)

include_directories(tokenizers/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(tokenizers)


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
