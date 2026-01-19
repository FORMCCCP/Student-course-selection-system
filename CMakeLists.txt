cmake_minimum_required(VERSION 4.1.1)

#https://github.com/Kitware/CMake/blob/v4.1.1/Help/dev/experimental.rst

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
set(CMAKE_CXX_MODULE_STD ON)

project(CourseManagement-system LANGUAGES CXX)


add_executable(CourseManagement-system
    #math.cpp                              # 模块实现单元
    main.cpp


)

set(DOMAIN_MODULES
    domain.cppm
    do.person.cppm
    do.student.cppm
    do.teacher.cppm
    do.course.cppm

    do.secretary.cppm







)
set(BROKER
    Database.cppm
    broker.cppm
    studentbroker.cppm
    teacherbroker.cppm
    secretarybroker.cppm
    coursebroker.cppm
    enrollmentbroker.cppm
)

set(UI
    userui.cppm
    studentui.cppm
    teacherui.cppm
    secretaryui.cppm
)

set(SERVICE
    ser.student.cppm
    ser.teacher.cppm
    ser.secretary.cppm
)
source_group("domain-module" FILES ${DOMAIN_MODULES})
source_group("broker-module" FILES ${BROKER})
source_group("ui-module" FILES ${UI})
source_group("service-module" FILES ${SERVICE})

target_sources(CourseManagement-system  PRIVATE FILE_SET cxx_modules TYPE CXX_MODULES
    BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        FILES ${DOMAIN_MODULES}
            ${BROKER}
            ${UI}
            ${SERVICE}
    )

target_compile_features(CourseManagement-system PRIVATE cxx_std_23)



target_include_directories(CourseManagement-system PRIVATE
    /usr/local/include
)

target_link_directories(CourseManagement-system PRIVATE
    /usr/local/lib)
target_link_libraries(CourseManagement-system PRIVATE
    pqxx
    pq
)

if(PQXX_LIB AND PQ_LIB)
    target_link_libraries(CourseManagement-system PRIVATE ${PQXX_LIB} ${PQ_LIB})
elseif(PQXX_FOUND)
    target_include_directories(CourseManagement-system PRIVATE ${PQXX_INCLUDE_DIRS})
    target_link_libraries(CourseManagement-system PRIVATE ${PQXX_LIBRARIES} PostgreSQL::PostgreSQL)
else()
    target_link_libraries(CourseManagement-system PRIVATE pqxx pq)
endif()
