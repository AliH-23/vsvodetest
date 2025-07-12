cmake_minimum_required(VERSION 3.16)

project(vsvodetest VERSION 0.1 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# حل مشكلة تعريف __cplusplus مع MSVC وQt6
if (MSVC)
    add_compile_options(/Zc:__cplusplus)
    add_compile_options(/permissive-)
    add_compile_options(/FS)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(TS_FILES
    translations/ar.ts
    translations/en.ts
)

qt_standard_project_setup()

qt_add_executable(vsvodetest
    main.cpp
    mainwindow.cpp
    mainwindow.h
    ProductsWindow.cpp
    ProductsWindow.h
    CustomersWindow.cpp
    CustomersWindow.h
    SalesWindow.cpp
    SalesWindow.h
    InstallmentsWindow.cpp
    InstallmentsWindow.h
    ReportsWindow.cpp
    ReportsWindow.h
    UserManager.cpp
    UserManager.h
    LoginWindow.cpp
    LoginWindow.h
    models.h
)


# توليد ملفات qm تلقائياً عند البناء
find_program(LRELEASE_EXECUTABLE lrelease)
if(LRELEASE_EXECUTABLE)
    foreach(_ts_file ${TS_FILES})
        get_filename_component(_qm_file ${_ts_file} NAME_WE)
        add_custom_command(
            OUTPUT ${CMAKE_SOURCE_DIR}/translations/${_qm_file}.qm
            COMMAND ${LRELEASE_EXECUTABLE} ${CMAKE_SOURCE_DIR}/${_ts_file} -qm ${CMAKE_SOURCE_DIR}/translations/${_qm_file}.qm
            DEPENDS ${CMAKE_SOURCE_DIR}/${_ts_file}
            COMMENT "Generating translation ${_qm_file}.qm"
        )
        list(APPEND QM_FILES ${CMAKE_SOURCE_DIR}/translations/${_qm_file}.qm)
    endforeach()
    add_custom_target(translations ALL DEPENDS ${QM_FILES})
endif()
target_link_libraries(vsvodetest PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(vsvodetest PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS vsvodetest
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
