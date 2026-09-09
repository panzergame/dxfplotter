if (MSVC)
    add_compile_options(/translateInclude)
endif()

function (add_module_library lib_name)
    cmake_parse_arguments(AML "SHARED" "" "NON_MODULE_SOURCES" ${ARGN})
    set(sources ${AML_UNPARSED_ARGUMENTS})

    if (AML_SHARED)
        add_library(${lib_name} SHARED ${AML_NON_MODULE_SOURCES})
    else()
        add_library(${lib_name} STATIC ${AML_NON_MODULE_SOURCES})
    endif()

    # BASE_DIRS is configured to allow files coming from src and build directory (for instance compile generated sources located in build/).
    target_sources(${lib_name} PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES ${sources} BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

    target_compile_features(${lib_name} PUBLIC cxx_std_23)

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${lib_name} PRIVATE
            -Wno-reserved-module-identifier
            -Wno-module-file-config-mismatch
        )
    endif()
endfunction()
