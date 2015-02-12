
set(EDITOR_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(EDITOR_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(EDITOR_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

QT5_WRAP_UI(EDITOR_UIS_FILES
    ${EDITOR_RESOURCES_DIR}/Editor.ui)

set(EDITOR_HEADER_FILES 
    ${EDITOR_HEADER_DIR}/Editor.h
    ${EDITOR_HEADER_DIR}/LevelScene.h
    ${EDITOR_HEADER_DIR}/PropertyRequests.h
    ${EDITOR_HEADER_DIR}/PropertyDelegate.h
    ${EDITOR_HEADER_DIR}/PropertyPopulate.h
    ${EDITOR_HEADER_DIR}/PropertyApply.h
    ${EDITOR_HEADER_DIR}/PropertyRoles.h)
set(EDITOR_SOURCE_FILES 
    ${EDITOR_SOURCE_DIR}/main.cpp
    ${EDITOR_SOURCE_DIR}/Editor.cpp
    ${EDITOR_SOURCE_DIR}/LevelScene.cpp
    ${EDITOR_SOURCE_DIR}/PropertyDelegate.cpp
    ${EDITOR_SOURCE_DIR}/PropertyPopulate.cpp
    ${EDITOR_SOURCE_DIR}/PropertyApply.cpp)

source_group("editor\\headers" FILES ${EDITOR_HEADER_FILES})
source_group("editor\\sources" FILES ${EDITOR_SOURCE_FILES})

add_executable(Editor WIN32 ${EDITOR_HEADER_FILES} ${EDITOR_SOURCE_FILES} ${EDITOR_UIS_FILES})
target_link_libraries(Editor game)
target_include_directories(Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

qt5_use_modules(Editor Widgets)