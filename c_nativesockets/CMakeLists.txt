cmake_minimum_required(VERSION 4.0)

project(nativesockets
    VERSION 1.0.0
    DESCRIPTION "Native UDP socket implementation"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)

# References the c_nativesocketpal library without modifying it.
set(NATIVESOCKETPAL_STATIC ON CACHE BOOL "Build the static library")
add_subdirectory(../c_nativesocketpal nativesocketpal-build)

set(NATIVESOCKETS_SOURCES
    _nativesockets.c
)

set(NATIVESOCKETS_PUBLIC_HEADERS
    include/_nativesockets.h
    include/_WinSock2.h
    include/_NativeSocket.h
    include/_NativeSocketAddress.h
)

option(NATIVESOCKETS_STATIC "Build the static library" OFF)
option(NATIVESOCKETS_SHARED "Build the shared library" OFF)

if(WIN32)
    set(NATIVESOCKETS_PLATFORM_LIBS ws2_32)
endif()

function(nativesockets_configure target)
    target_include_directories(${target} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
    target_link_libraries(${target} PUBLIC nativesocketpal_static)
    if(WIN32)
        target_link_libraries(${target} PUBLIC ${NATIVESOCKETS_PLATFORM_LIBS})
    endif()
    if(MSYS OR MINGW)
        target_compile_definitions(${target} PRIVATE
            WINVER=0x0601
            _WIN32_WINNT=0x0601
        )
        target_link_options(${target} PRIVATE -static)
    endif()
endfunction()

if(NATIVESOCKETS_STATIC)
    add_library(nativesockets_static STATIC ${NATIVESOCKETS_SOURCES} ${NATIVESOCKETS_PUBLIC_HEADERS})
    nativesockets_configure(nativesockets_static)
    if(WIN32)
        set_target_properties(nativesockets_static PROPERTIES OUTPUT_NAME nativesockets_static)
    else()
        set_target_properties(nativesockets_static PROPERTIES OUTPUT_NAME nativesockets)
    endif()
    set_target_properties(nativesockets_static PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

if(NATIVESOCKETS_SHARED)
    add_library(nativesockets SHARED ${NATIVESOCKETS_SOURCES} ${NATIVESOCKETS_PUBLIC_HEADERS})
    nativesockets_configure(nativesockets)
    target_compile_definitions(nativesockets PRIVATE _NATIVESOCKETS_BUILD_DLL)
    set_target_properties(nativesockets PROPERTIES OUTPUT_NAME nativesockets)
endif()

install(FILES ${NATIVESOCKETS_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if(NATIVESOCKETS_STATIC)
    install(TARGETS nativesockets_static ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

if(NATIVESOCKETS_SHARED)
    install(TARGETS nativesockets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()
