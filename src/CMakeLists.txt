cmake_minimum_required(VERSION 3.25)

find_package(mo2-cmake CONFIG REQUIRED)
find_package(mo2-uibase CONFIG REQUIRED)
find_package(Qt6 CONFIG REQUIRED COMPONENTS Core Widgets)

add_executable(nxmhandler)
mo2_configure_target(nxmhandler)
set_target_properties(nxmhandler PROPERTIES WIN32_EXECUTABLE TRUE)
target_link_libraries(nxmhandler PRIVATE Qt6::Core Qt6::Widgets mo2::uibase)

include(GNUInstallDirs)
install(TARGETS nxmhandler DESTINATION ${CMAKE_INSTALL_BINDIR})
mo2_install_pdb(TARGET nxmhandler)

if (WIN32)
    mo2_deploy_qt(BINARIES nxmhandler.exe NOPLUGINS)
else ()
    set_target_properties(nxmhandler PROPERTIES INSTALL_RPATH "$ORIGIN;$ORIGIN/lib")
endif()
