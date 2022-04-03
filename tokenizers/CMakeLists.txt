# Enable modules to include each other's files
include_directories(.)

add_subdirectory(fundamental)
add_subdirectory(basic)
add_subdirectory(bert)
add_subdirectory(lib)
add_subdirectory(utils)

add_library(tokenizers INTERFACE)
target_link_libraries(tokenizers INTERFACE
    fundamental_tokenizer
    basic_tokenizers
    bert_tokenizer
    utils
    unilib
    )
target_include_directories(tokenizers INTERFACE
    "$<BUILD_INTERFACE:${TOKENIZERS_COMMON_INCLUDE_DIRS}>"
    $<INSTALL_INTERFACE:include>
    )

set_target_properties(tokenizers PROPERTIES
    INTERFACE_COMPILE_DEFINITIONS "U_STATIC_IMPLEMENTATION;U_CHARSET_IS_UTF8=1;U_NO_DEFAULT_INCLUDE_UTF_HEADERS=1")

add_library(tokenizers::tokenizers ALIAS tokenizers)
