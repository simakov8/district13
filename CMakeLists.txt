cmake_minimum_required(VERSION 3.26)
project("District13")

list(APPEND CMAKE_PREFIX_PATH "D:/Qt/6.2.4/msvc2019_64")
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)


set(sources main.cpp
            gui/mainwindow.cpp
            core/game.cpp
            core/district.cpp
            core/sites/building.cpp
            core/administration.cpp
            core/sites/site.cpp
            core/sites/house.cpp
            gui/minimap.cpp
            core/factor.cpp
            gui/siteinfodialog.cpp
            core/events/event.cpp
            gui/eventlogger.cpp
            core/sites/publicbuilding.cpp
            gui/builddialog.cpp
            gui/gameinfowidget.cpp
            gui/actionswidget.cpp
            res/resources.qrc)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core Qt6::Widgets)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
  COMMAND_EXPAND_LISTS
)

