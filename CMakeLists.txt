cmake_minimum_required(VERSION 3.11)
project(pvd_custom)

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

set(TARGET pvd_custom)

set(HEADERS)
set(SOURCES)
set(UI)
file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE UI ${CMAKE_SOURCE_DIR}/src/*.ui)
source_group(TREE "${CMAKE_SOURCE_DIR}" FILES ${HEADERS} ${SOURCES} ${UI})

include_directories(${CMAKE_SOURCE_DIR}/3rd)
include_directories(${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Include)
include_directories(${CMAKE_SOURCE_DIR}/3rd/PhysX/PxShared/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

message(STATUS "find qt package")
set(CMAKE_PREFIX_PATH "$ENV{QTDIR512}")  #set qt env path
find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL REQUIRED)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/src/ui)  #required
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
add_executable(${TARGET} ${HEADERS} ${SOURCES} ${UI})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "build with debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    target_link_libraries(${TARGET}
        "${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Lib/vc15win64/*DEBUG*.lib"
        "${CMAKE_SOURCE_DIR}/3rd/PhysX/PxShared/lib/vc15win64/*DEBUG*.lib"
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::OpenGL
        OpenGL32.lib
    )
    set(debug_dll)
    file(GLOB_RECURSE debug_dll
        ${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Bin/vc15win64/nvToolsExt64_1.dll
        ${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Bin/vc15win64/*DEBUG*
        ${CMAKE_SOURCE_DIR}/3rd/PhysX/PxShared/bin/vc15win64/*DEBUG*
        ${CMAKE_PREFIX_PATH}/bin/Qt5Cored.dll
        ${CMAKE_PREFIX_PATH}/bin/Qt5Guid.dll
        ${CMAKE_PREFIX_PATH}/bin/Qt5Widgetsd.dll
    )
    foreach(file_i ${debug_dll})
        add_custom_command(TARGET ${TARGET} POST_BUILD 
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${file_i}
            ${CMAKE_SOURCE_DIR}/bin/$<CONFIGURATION>
        )
    endforeach(file_i)
else()
    message(STATUS "build with release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /O3")
    target_link_libraries(${TARGET}
        "${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Lib/vc15win64/LowLevel.lib"
        "${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Lib/vc15win64/LowLevelAABB.lib"
        "${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Lib/vc15win64/LowLevelCloth.lib"
        "${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Lib/vc15win64/LowLevelDynamics.lib"
        "${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Lib/vc15win64/LowLevelParticles.lib"
        "${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Lib/vc15win64/PhysX3_x64.lib"
        "${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Lib/vc15win64/PhysX3CharacterKinematic_x64.lib"
        "${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Lib/vc15win64/PhysX3Common_x64.lib"
        "${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Lib/vc15win64/PhysX3Cooking_x64.lib"
        "${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Lib/vc15win64/PhysX3Extensions.lib"
        "${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Lib/vc15win64/PhysX3Vehicle.lib"
        "${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Lib/vc15win64/SceneQuery.lib"
        "${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Lib/vc15win64/SimulationController.lib"
        "${CMAKE_SOURCE_DIR}/3rd/PhysX/PxShared/lib/vc15win64/PsFastXml_x64.lib"
        "${CMAKE_SOURCE_DIR}/3rd/PhysX/PxShared/lib/vc15win64/PxFoundation_x64.lib"
        "${CMAKE_SOURCE_DIR}/3rd/PhysX/PxShared/lib/vc15win64/PxPvdSDK_x64.lib"
        "${CMAKE_SOURCE_DIR}/3rd/PhysX/PxShared/lib/vc15win64/PxTask_x64.lib"
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::OpenGL
        OpenGL32.lib
    )
    set(release_dll)
    file(GLOB_RECURSE release_dll
        ${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Bin/vc15win64/nvToolsExt64_1.dll
        ${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Bin/vc15win64/PhysX3_x64.dll
        ${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Bin/vc15win64/PhysX3CharacterKinematic_x64.dll
        ${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Bin/vc15win64/PhysX3Common_x64.dll
        ${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Bin/vc15win64/PhysX3Cooking_x64.dll
        ${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Bin/vc15win64/PhysX3Gpu_x64.dll
        ${CMAKE_SOURCE_DIR}/3rd/PhysX/PhysX_3.4/Bin/vc15win64/PhysXDevice64.dll
        ${CMAKE_SOURCE_DIR}/3rd/PhysX/PxShared/bin/vc15win64/PxFoundation_x64.dll
        ${CMAKE_SOURCE_DIR}/3rd/PhysX/PxShared/bin/vc15win64/PxPvdSDK_x64.dll
        ${CMAKE_PREFIX_PATH}/bin/Qt5Core.dll
        ${CMAKE_PREFIX_PATH}/bin/Qt5Gui.dll
        ${CMAKE_PREFIX_PATH}/bin/Qt5Widgets.dll
    )
    foreach(file_i ${release_dll})
        add_custom_command(TARGET ${TARGET} POST_BUILD 
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${file_i}
            ${CMAKE_SOURCE_DIR}/bin/$<CONFIGURATION>
        )
    endforeach(file_i)
endif()

target_compile_options(${TARGET} PRIVATE /W4 /D_CRT_SECURE_NO_WARNINGS)
target_compile_options(${TARGET} PRIVATE /fp:fast)

set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ${TARGET})
