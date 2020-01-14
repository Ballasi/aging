cmake_minimum_required (VERSION 2.6)

project (Hashlife)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets 3DCore 3DExtras 3DRender 3DInput Charts)

AUX_SOURCE_DIRECTORY(src SRC1)
AUX_SOURCE_DIRECTORY(src/logic SRC2) 
AUX_SOURCE_DIRECTORY(src/ui SRC3)

add_executable(Hashlife ${SRC1} ${SRC2} ${SRC3})

target_link_libraries(Hashlife PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::3DCore
    Qt5::3DExtras
    Qt5::3DRender
    Qt5::3DInput
    Qt5Charts
    )
