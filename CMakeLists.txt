cmake_minimum_required(VERSION 3.16)

project(graph VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resources/icon.rc")

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(graph
        src/main.cpp
        src/mainwindow.cpp
        src/tintinglabel.cpp
        src/highlightedlineedit.cpp

        ${app_icon_resource_windows}
)
target_link_libraries(graph PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(graph PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS graph
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
