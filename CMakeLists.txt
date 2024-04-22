cmake_minimum_required(VERSION 3.7)

project(KnucklesBugFix)
set(MOD_NAME KnucklesBugFix CACHE STRING "The mod directory to look into")

add_library(KnucklesBugFix SHARED
    GameAPI/C/GameAPI/Game.c
    ${MOD_NAME}/dllmain.c
    ${MOD_NAME}/Objects/Menu/LevelSelect.c
)

target_include_directories(KnucklesBugFix PRIVATE
    ${MOD_NAME}/
    GameAPI/C/
)
set_target_properties(KnucklesBugFix PROPERTIES OUTPUT_NAME "${MOD_NAME}")

if(ANDROID)
    set_target_properties(KnucklesBugFix PROPERTIES PREFIX "lib")
else()
    set_target_properties(KnucklesBugFix PROPERTIES PREFIX "")
endif()
    
unset(MOD_NAME CACHE)
unset(OUTPUT_NAME CACHE)