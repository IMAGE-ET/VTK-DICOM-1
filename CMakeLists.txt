cmake_minimum_required(VERSION 2.4)

project(Dialog-for-file)

ADD_DEFINITIONS (${QT_DEFINITIONS})

SET(SOURCES
filedialog.cpp
main.cpp
)



SET( QT_USE_QTXML TRUE )
SET( QT_USE_QTOPENGL TRUE )
SET( QT_WRAP_CPP TRUE )

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

QT4_WRAP_UI(UIScrs filedialog.ui)
QT4_WRAP_CPP(MOCScrs filedialog.h)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

#SET(file(GLOB SOURCES "*.cpp"))

include_directories(${MOCScrs} ${UIScrs} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(Dialog-for-file ${SOURCES} ${MOCScrs} ${UIScrs})
target_link_libraries(Dialog-for-file ${QT_LIBRARIES} ${ITK_LIBRARIES} ${VTK_LIBRARIES})

