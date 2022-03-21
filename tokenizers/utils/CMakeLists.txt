add_library(utils OBJECT
    tokenizer_utils.h
    tokenizer_utils.cc
    unistr_utils.h
    )

target_link_libraries(utils PUBLIC
    unilib
    ${ICU_LIBRARIES}
    )

# target_include_directories(tokenizers INTERFACE
#     $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/tokenizers>
#     $<INSTALL_INTERFACE:include>
#     )

set_target_properties(utils PROPERTIES
    COMPILE_DEFINITIONS "U_STATIC_IMPLEMENTATION;U_CHARSET_IS_UTF8=1;U_NO_DEFAULT_INCLUDE_UTF_HEADERS=1")
