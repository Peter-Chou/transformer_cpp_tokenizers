# Enable modules to include each other's files
include_directories(.)

add_subdirectory(basic)
add_subdirectory(bert)
add_subdirectory(unilib)
add_subdirectory(utils)

add_executable(main
    main.cc
    )

target_link_libraries(main PUBLIC
    bert_tokenizer
    basic_tokenizer
    utils
    unilib
    ${ICU_LIBRARIES}
    )
target_include_directories(main PUBLIC ${ICU_INCLUDE_DIRS})

set_target_properties(main PROPERTIES
    INTERFACE_COMPILE_DEFINITIONS "U_STATIC_IMPLEMENTATION;U_CHARSET_IS_UTF8=1;U_NO_DEFAULT_INCLUDE_UTF_HEADERS=1")
