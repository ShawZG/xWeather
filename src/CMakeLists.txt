SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

#SET(Qt5_DIR "${CMAKE_PREFIX_PATH}/cmake/Qt5")
#SET(Qt5Core_DIR "${CMAKE_PREFIX_PATH}/cmake/Qt5Core")
#SET(Qt5Gui_DIR "${CMAKE_PREFIX_PATH}/cmake/Qt5Gui")
#SET(Qt5Widgets_DIR "${CMAKE_PREFIX_PATH}/cmake/Qt5Widgets")
#SET(Qt5WebEngineWidgets_DIR "${CMAKE_PREFIX_PATH}/cmake/Qt5WebEngineWidgets")
#FIND_PACKAGE失败则需要设置CMAKE_PREFIX_PATH，或者设置上述路径
IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    SET(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake")
    SET(CMAKE_INSTALL_PREFIX /usr)
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    SET(Qt5_DIR "D:/Qt/Qt5.12.11/5.12.11/mingw73_64/lib/cmake/Qt5")
ENDIF()

FIND_PACKAGE(Qt5 COMPONENTS Core REQUIRED)
FIND_PACKAGE(Qt5 COMPONENTS Gui REQUIRED)
FIND_PACKAGE(Qt5 COMPONENTS Widgets REQUIRED)
FIND_PACKAGE(Qt5 COMPONENTS Network REQUIRED)
IF(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    FIND_PACKAGE(Qt5 COMPONENTS X11Extras REQUIRED)
    FIND_PACKAGE(X11 REQUIRED)
ENDIF()

#Application单例库
SET(QAPPLICATION_CLASS QApplication CACHE STRING "Inheritance class for SingleApplication")
ADD_SUBDIRECTORY(${CMAKE_CURRENT_LIST_DIR}/third_party/SingleApplication)
SET(3RD_LIBS SingleApplication::SingleApplication)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/common/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/common/version.h @ONLY)
FILE(GLOB_RECURSE SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.h)
#已经使用了SET(CMAKE_AUTORCC ON)，可以不使用QT5_ADD_RESOURCES, qrc文件最终也是被编译为CPP文件。
QT5_ADD_RESOURCES(QRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resource/res.qrc)

#Windows平台图标
IF(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    SET(WINDOWS_RC_FILE ${PROJECT_SOURCE_DIR}/data/app.rc)
ENDIF()

SET(APP_NAME ${PROJECT_NAME})
#将编译产生的可执行文件或库文件存放到指定目录下，不包含编译生成的中间文件
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#This is similar to qt5_add_resources, but directly generates object files (.o, .obj) files instead of C++ source code.
#This allows to embed bigger resources, where compiling to C++ sources and then to binaries would be too time consuming or memory intensive.
#QT5_ADD_BIG_RESOURCES(${APP_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/resource/res.qrc)

ADD_EXECUTABLE(${APP_NAME} ${SRC_FILES} ${QRC_FILES} ${WINDOWS_RC_FILE})
TARGET_INCLUDE_DIRECTORIES(${APP_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/common)
TARGET_INCLUDE_DIRECTORIES(${APP_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dialogs)
TARGET_INCLUDE_DIRECTORIES(${APP_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/forecast_view)
IF(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    TARGET_LINK_LIBRARIES(${APP_NAME} Qt5::Core Qt5::Widgets Qt5::Network Qt5::X11Extras ${X11_LIBRARIES} ${3RD_LIBS})
ELSE()
    TARGET_LINK_LIBRARIES(${APP_NAME} Qt5::Core Qt5::Widgets Qt5::Network ${3RD_LIBS})
ENDIF()

INSTALL(TARGETS ${APP_NAME} RUNTIME                               DESTINATION /opt/apps/org.xzg.xweather/files/)
INSTALL(FILES ${PROJECT_SOURCE_DIR}/data/org.xzg.xweather.desktop DESTINATION /opt/apps/org.xzg.xweather/entries/applications/)
INSTALL(FILES ${PROJECT_SOURCE_DIR}/data/org.xzg.xweather.desktop DESTINATION /usr/share/applications)
INSTALL(FILES ${PROJECT_SOURCE_DIR}/data/org.xzg.xweather.svg     DESTINATION /opt/apps/org.xzg.xweather/entries/icons/hicolor/scalable/apps/)
INSTALL(FILES ${PROJECT_SOURCE_DIR}/data/info                     DESTINATION /opt/apps/org.xzg.xweather/)
#构建DEB包
SET(CPACK_GENERATOR "DEB")
SET(CPACK_PACKAGE_NAME "org.xzg.xweather")
SET(CPACK_PACKAGE_CONTACT "blueshaw@163.com")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Xiao Zhiguo")
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "A simple weather app")
SET(CPACK_DEBIAN_PACKAGE_VERSION ${PACKAGE_VERSION})
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "Optional")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5core5a, libqt5widgets5, libqt5network5, libqt5gui5")
INCLUDE(CPack)