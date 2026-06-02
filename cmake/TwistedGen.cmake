# TwistedGen.cmake
# Integrates the twistedgen.py pre-build code generator.
#
# Usage in a CMakeLists.txt:
#
#   twisted_gen_target(
#       TARGET       Editor                          # CMake target to attach gen files to
#       INCLUDE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/include
#       GEN_DIR      ${CMAKE_BINARY_DIR}/Generated/Editor
#       INCLUDE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/include
#   )

find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(TWISTEDGEN_SCRIPT "${CMAKE_SOURCE_DIR}/tools/twistedgen.py")

function(twisted_gen_target)
    cmake_parse_arguments(ARG "" "TARGET;INCLUDE_DIR;GEN_DIR;INCLUDE_ROOT" "" ${ARGN})

    if(NOT ARG_TARGET OR NOT ARG_INCLUDE_DIR OR NOT ARG_GEN_DIR OR NOT ARG_INCLUDE_ROOT)
        message(FATAL_ERROR "[TwistedGen] twisted_gen_target requires TARGET, INCLUDE_DIR, GEN_DIR, INCLUDE_ROOT")
    endif()

    set(CMAKE_GEN_FILE "${ARG_GEN_DIR}/generated.cmake")

    # ── Configure-time: full scan ─────────────────────────────────────────────
    # Runs once when CMake configures, creating all .gen.cpp files.
    message(STATUS "[TwistedGen] Scanning ${ARG_INCLUDE_DIR} for ${ARG_TARGET}...")
    execute_process(
        COMMAND "${Python3_EXECUTABLE}" "${TWISTEDGEN_SCRIPT}"
                scan
                "${ARG_INCLUDE_DIR}"
                "${ARG_GEN_DIR}"
                "${CMAKE_GEN_FILE}"
                "${ARG_INCLUDE_ROOT}"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        RESULT_VARIABLE GEN_RESULT
    )
    if(NOT GEN_RESULT EQUAL "0")
        message(FATAL_ERROR "[TwistedGen] Generator failed for target ${ARG_TARGET}")
    endif()

    # ── Include the generated source list ─────────────────────────────────────
    include("${CMAKE_GEN_FILE}")   # sets TWISTED_GEN_SOURCES

    if(NOT TWISTED_GEN_SOURCES)
        message(STATUS "[TwistedGen] No annotated classes found for ${ARG_TARGET}")
        return()
    endif()

    target_sources(${ARG_TARGET} PRIVATE ${TWISTED_GEN_SOURCES})

    # ── Build-time: re-run generator when any header changes ──────────────────
    # If a header already marked TCLASS changes, the gen file is rebuilt automatically.
    # If a NEW TCLASS is added to a header, re-run CMake once to pick it up.
    file(GLOB_RECURSE _ALL_HEADERS "${ARG_INCLUDE_DIR}/*.h")

    add_custom_command(
        OUTPUT  ${TWISTED_GEN_SOURCES} "${CMAKE_GEN_FILE}"
        COMMAND "${Python3_EXECUTABLE}" "${TWISTEDGEN_SCRIPT}"
                scan
                "${ARG_INCLUDE_DIR}"
                "${ARG_GEN_DIR}"
                "${CMAKE_GEN_FILE}"
                "${ARG_INCLUDE_ROOT}"
        DEPENDS ${_ALL_HEADERS}
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "[TwistedGen] Regenerating gen files for ${ARG_TARGET}"
    )
endfunction()
