cmake_minimum_required(VERSION 3.25)

if (LINUX)
    find_package(OpenGL REQUIRED)
endif()

find_package(mo2-cmake CONFIG REQUIRED)
find_package(mo2-uibase CONFIG REQUIRED)
find_package(Qt6 CONFIG REQUIRED COMPONENTS Core Widgets)

add_executable(nxmhandler)
mo2_configure_target(nxmhandler)
set_target_properties(nxmhandler PROPERTIES WIN32_EXECUTABLE TRUE)
target_link_libraries(nxmhandler PRIVATE Qt6::Core Qt6::Widgets mo2::uibase)

install(FILES $<TARGET_FILE:nxmhandler> DESTINATION bin
    PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE
)
if (WIN32)
    install(FILES $<TARGET_PDB_FILE:nxmhandler> DESTINATION pdb)
endif()

mo2_deploy_qt(BINARIES nxmhandler.exe NOPLUGINS)
