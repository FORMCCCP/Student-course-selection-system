cmake_minimum_required(VERSION 4.1.1)

#https://github.com/Kitware/CMake/blob/v4.1.1/Help/dev/experimental.rst

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
set(CMAKE_CXX_MODULE_STD ON)

project(CourseManagement-system LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(MODULE_LIB_DIR1 /opt/utils/modules)  # 模块库的目录的设置
add_executable(CourseManagement-system
    #math.cpp                              # 模块实现单元
    main.cpp
    Database.h
    studentbroker.h


)

set(DOMAIN_MODULES
    domain.cppm
    do.person.cppm
    do.student.cppm
    do.teacher.cppm
    do.course.cppm
    do.enroll.cppm
    do.secretary.cppm
)

set(UI_MODULES
    ui.cppm
)

set(SYSTEM_MODULES
    system.cppm
)

source_group("Module Interfaces" FILES ${MODULE_INTERFACE_FILES})
source_group("domain" FILES ${DOMAIN_MODULES})
source_group("ui" FILES ${UI_MODULES})
source_group("system" FILES ${SYSTEM_MODULES})

target_sources(CourseManagement-system  PRIVATE FILE_SET cxx_modules TYPE CXX_MODULES
    BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        FILES ${MODULE_INTERFACE_FILES} ${DOMAIN_MODULES} ${UI_MODULES} ${SYSTEM_MODULES}
)
target_compile_features(CourseManagement-system PRIVATE cxx_std_23)

target_include_directories(CourseManagement-system PRIVATE
    /usr/include
)

if(PQXX_LIB AND PQ_LIB)
    target_link_libraries(CourseManagement-system PRIVATE ${PQXX_LIB} ${PQ_LIB})
elseif(PQXX_FOUND)
    target_include_directories(CourseManagement-system PRIVATE ${PQXX_INCLUDE_DIRS})
    target_link_libraries(CourseManagement-system PRIVATE ${PQXX_LIBRARIES} PostgreSQL::PostgreSQL)
else()
    target_link_libraries(Base PRIVATE pqxx pq)
endif()
