add_library(unilib
    unicode.h
    unicode.cpp
    uninorms.h
    uninorms.cpp
    )
target_include_directories(unilib PUBLIC
    "$<BUILD_INTERFACE:${TOKENIZERS_COMMON_INCLUDE_DIRS}>"
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
if(TOKENIZERS_ENABLE_INSTALL)
    install(TARGETS unilib
        EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )
endif()

add_library(tokenizers::lib::unilib ALIAS unilib)
