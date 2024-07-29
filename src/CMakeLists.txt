cmake_minimum_required(VERSION 3.16)

find_package(mo2-cmake CONFIG REQUIRED)
find_package(mo2-uibase CONFIG REQUIRED)
find_package(Qt6 CONFIG REQUIRED COMPONENTS Core Widgets)

if(WIN32)
    set(OS_SPECIFIC_DIR win32)
else()
    set(OS_SPECIFIC_DIR linux)
endif()

set(SOURCES
    addbinarydialog.cpp
    handlerstorage.cpp
    ${OS_SPECIFIC_DIR}/handlerstorage.cpp
    handlerwindow.cpp
    logger.cpp
    main.cpp
    ${OS_SPECIFIC_DIR}/utility.cpp
)
add_executable(nxmhandler)
mo2_configure_target(nxmhandler NO_SOURCES)
target_sources(nxmhandler PRIVATE ${SOURCES})
set_target_properties(nxmhandler PROPERTIES WIN32_EXECUTABLE TRUE)
target_link_libraries(nxmhandler PRIVATE Qt6::Core Qt6::Widgets mo2::uibase)

install(FILES $<TARGET_FILE:nxmhandler> DESTINATION bin)
if (WIN32)
    install(FILES $<TARGET_PDB_FILE:nxmhandler> DESTINATION pdb)

    mo2_deploy_qt(BINARIES nxmhandler.exe NOPLUGINS)
endif()
