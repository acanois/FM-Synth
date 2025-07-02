cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME "FMSynth")
set(FORMATS AU Standalone)

file(STRINGS VERSION CURRENT_VERSION)

project(${PROJECT_NAME} VERSION ${CURRENT_VERSION})

set(CMAKE_XCODE_GENERATE_SCHEME OFF)

if ((DEFINED ENV{CI}) AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    message("Building for Apple Silicon and x86_64")
    set(CMAKE_OSX_ARCHITECTURES arm64 x86_64)
endif ()

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Support macOS down to High Sierra")

set_property(GLOBAL PROPERTY USE_FOLDERS YES)

option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Show all module sources in IDE projects" ON)

add_subdirectory(JUCE)

juce_add_plugin("${PROJECT_NAME}"
    # VERSION ...
    # ICON_BIG ...
    # ICON_SMALL ...
    COMPANY_NAME acanois
    BUNDLE_ID com.acanois.acanois
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    # NEEDS_MIDI_OUTPUT TRUE/FALSE
    # IS_MIDI_EFFECT TRUE/FALSE
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE # On MacOS, plugin will be copied to /Users/you/Library/Audio/Plug-Ins/
    PLUGIN_MANUFACTURER_CODE Acan
    PLUGIN_CODE P001
    FORMATS "${FORMATS}"
    PRODUCT_NAME "${PROJECT_NAME}")

# C++20, please
target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_20)

# Manually list all .h and .cpp files for the plugin
set(SourceFiles
    Source/PluginEditor.h
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginProcessor.cpp)
target_sources("${PROJECT_NAME}" PRIVATE ${SourceFiles})

# No, we don't want our source buried in extra nested folders
set_target_properties("${PROJECT_NAME}" PROPERTIES FOLDER "")

# The Xcode source tree should uhhh, still look like the source tree, yo
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX "" FILES ${SourceFiles})

foreach(target ${FORMATS} "All")
    if(TARGET ${PROJECT_NAME}_${target})
        set_target_properties(${PROJECT_NAME}_${target} PROPERTIES
            # Tuck the actual plugin targets into a folder where they won't bother us
            FOLDER "Targets"

            # MacOS only: Sets the default executable that Xcode will open on build
            # For this exact path to to work, manually build the AudioPluginHost.xcodeproj in the JUCE subdir
            XCODE_SCHEME_EXECUTABLE "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/extras/AudioPluginHost/Builds/MacOSX/build/Debug/AudioPluginHost.app"

            # Let us build the target in Xcode
            XCODE_GENERATE_SCHEME ON)
    endif()
endforeach()

# Uncomment for assets directory
#set_target_properties(Assets PROPERTIES FOLDER "Targets")


target_compile_definitions("${PROJECT_NAME}"
    PUBLIC
    # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
    JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries("${PROJECT_NAME}"
    PRIVATE
    #    Assets
    PUBLIC
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)

# When present, use Intel IPP for performance on Windows
if(WIN32) # Can't use MSVC here, as it won't catch Clang on Windows
    find_package(IPP)
    if(IPP_FOUND)
        target_link_libraries("${PROJECT_NAME}" PUBLIC IPP::ipps IPP::ippcore IPP::ippi IPP::ippcv)
        message("IPP LIBRARIES FOUND")
        target_compile_definitions("${PROJECT_NAME}" PUBLIC PAMPLEJUCE_IPP=1)
    else()
        message("IPP LIBRARIES *NOT* FOUND")
    endif()
endif()

enable_testing()

file(GLOB_RECURSE TestFiles CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Tests/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Tests/*.h")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Tests PREFIX "" FILES ${TestFiles})

Include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    GIT_TAG v3.3.2)
FetchContent_MakeAvailable(Catch2)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options (-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options (-fcolor-diagnostics)
endif ()
