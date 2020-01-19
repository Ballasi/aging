cmake_minimum_required (VERSION 2.6)

project (Hashlife)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets 3DCore 3DExtras 3DRender 3DInput Charts)

file(
    GLOB_RECURSE

    sources_files

    src/*
    )

add_executable(Hashlife ${sources_files})

target_link_libraries(Hashlife PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::3DCore
    Qt5::3DExtras
    Qt5::3DRender
    Qt5::3DInput
    Qt5::Charts
)
