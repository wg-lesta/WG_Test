
set(GAME_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/game)
set(GAME_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)

set(GAME_HEADER_FILES
    ${GAME_HEADER_DIR}/level.h
    ${GAME_HEADER_DIR}/enviroment.h
    ${GAME_HEADER_DIR}/prototype.h
    ${GAME_HEADER_DIR}/property.h
    ${GAME_HEADER_DIR}/object.h
    ${GAME_HEADER_DIR}/types.h
    ${GAME_HEADER_DIR}/forward.h)

set(GAME_SOURCE_FILES
    ${GAME_SOURCE_DIR}/level.cpp
    ${GAME_SOURCE_DIR}/enviroment.cpp
    ${GAME_SOURCE_DIR}/prototype.cpp
    ${GAME_SOURCE_DIR}/object.cpp)

set(GAME_SOURCE_PROPERTIES_FILE
    ${GAME_SOURCE_DIR}/properties/property_factory.cpp
    ${GAME_SOURCE_DIR}/properties/property_factory.h
    ${GAME_SOURCE_DIR}/properties/property_creators.h
    ${GAME_SOURCE_DIR}/properties/property_creators.cpp
    ${GAME_SOURCE_DIR}/properties/property_reader.h
    ${GAME_SOURCE_DIR}/properties/property_reader.cpp
    ${GAME_SOURCE_DIR}/properties/property_writer.h
    ${GAME_SOURCE_DIR}/properties/property_writer.cpp)

source_group("game\\headers\\game" FILES ${GAME_HEADER_FILES})
source_group("game\\sources" FILES ${GAME_SOURCE_FILES})
source_group("game\\sources\\properties" FILES ${GAME_SOURCE_PROPERTIES_FILE})

include_directories(${GAME_HEADER_DIR})

add_library(game STATIC 
    ${GAME_SOURCE_FILES}
    ${GAME_HEADER_FILES} 
    ${GAME_SOURCE_PROPERTIES_FILE})

set(JSON_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/common/jsoncpp)
target_link_libraries(game debug ${JSON_LIBRARY}/lib/lib_jsond.lib)
target_link_libraries(game optimized ${JSON_LIBRARY}/lib/lib_json.lib)

target_include_directories(game PRIVATE ${JSON_LIBRARY}/include)

target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)