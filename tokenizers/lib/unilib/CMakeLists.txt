add_library(unilib OBJECT
    unicode.h
    unicode.cpp
    uninorms.h
    uninorms.cpp
    )
target_include_directories(unilib PUBLIC
    "$<BUILD_INTERFACE:${TOKENIZERS_COMMON_INCLUDE_DIRS}>"
    $<INSTALL_INTERFACE:include>
    )
add_library(tokenizers::lib::unilib ALIAS unilib)
