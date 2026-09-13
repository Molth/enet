cmake_minimum_required(VERSION 4.0)

project(enet
    VERSION 1.0.0
    DESCRIPTION "ENet reliable UDP networking library"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)

# References the c_nativesockets library without modifying it.
set(NATIVESOCKETS_STATIC ON CACHE BOOL "Build the static library")
add_subdirectory(../c_nativesockets nativesockets-build)

set(ENET_SOURCES
    callbacks.c
    compress.c
    host.c
    list.c
    packet.c
    peer.c
    protocol.c
    win32.c
)

set(ENET_PUBLIC_HEADERS
    include/enet/callbacks.h
    include/enet/enet.h
    include/enet/list.h
    include/enet/protocol.h
    include/enet/time.h
    include/enet/types.h
    include/enet/utility.h
    include/enet/win32.h
)

option(ENET_STATIC "Build the static library" OFF)
option(ENET_SHARED "Build the shared library" OFF)

function(enet_configure target)
    target_include_directories(${target} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
    target_link_libraries(${target} PUBLIC nativesockets_static)
    if(WIN32)
        target_link_libraries(${target} PRIVATE winmm)
    endif()
endfunction()

if(ENET_STATIC)
    add_library(enet_static STATIC ${ENET_SOURCES} ${ENET_PUBLIC_HEADERS})
    enet_configure(enet_static)
    if(WIN32)
        set_target_properties(enet_static PROPERTIES OUTPUT_NAME enet_static)
    else()
        set_target_properties(enet_static PROPERTIES OUTPUT_NAME enet)
    endif()
    set_target_properties(enet_static PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

if(ENET_SHARED)
    add_library(enet SHARED ${ENET_SOURCES} ${ENET_PUBLIC_HEADERS})
    enet_configure(enet)
    if(WIN32 AND (MSVC OR CMAKE_C_COMPILER_ID MATCHES "Clang"))
        target_compile_definitions(enet PRIVATE
            ENET_DLL=1
            ENET_BUILDING_LIB
        )
    endif()
    set_target_properties(enet PROPERTIES OUTPUT_NAME enet)
endif()

install(FILES ${ENET_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/enet)

if(ENET_STATIC)
    install(TARGETS enet_static ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

if(ENET_SHARED)
    install(TARGETS enet
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()
