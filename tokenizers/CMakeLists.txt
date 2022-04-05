# Enable modules to include each other's files
include_directories(.)

add_subdirectory(fundamental)
add_subdirectory(basic)
add_subdirectory(bert)
add_subdirectory(lib)
add_subdirectory(utils)

add_library(tokenizers INTERFACE)
target_link_libraries(tokenizers INTERFACE
    tokenizers::bert_tokenizer
    )
target_include_directories(tokenizers INTERFACE
    "$<BUILD_INTERFACE:${TOKENIZERS_COMMON_INCLUDE_DIRS}>"
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

if(TOKENIZERS_ENABLE_INSTALL)
    install(TARGETS tokenizers
        EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )
endif()

add_library(tokenizers::tokenizers ALIAS tokenizers)
