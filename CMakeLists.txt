cmake_minimum_required(VERSION 4.1.1)

#https://github.com/Kitware/CMake/blob/v4.1.1/Help/dev/experimental.rst

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
set(CMAKE_CXX_MODULE_STD ON)

project(CourseManagement-system LANGUAGES CXX)


set(UI_HEADERS
    userui.h
    studentui.h
    teacherui.h
    secretaryui.h
)

set(BROKER_HEADER
    Database.h
    studentbroker.h
    coursebroker.h
    enrollmentbroker.h
    teacherbroker.h
    secretarybroker.h
    broker.h
)

set(SERVICE_HEADER
    ser.student.h
    ser.secretary.h
    ser.teacher.h
)
source_group("uis-head" FILES ${UI_HEADERS})
source_group("broker-head" FILES ${BROKER_HEADER})
source_group("service-head" FILES ${SERVICE_HEADER})

add_executable(CourseManagement-system
    #math.cpp                              # 模块实现单元
    main.cpp
    ${BROKER_HEADER}
    ${UI_HEADERS}
    ${SERVICE_HEADER}


)

set(DOMAIN_MODULES
    domain.cppm
    do.person.cppm
    do.student.cppm
    do.teacher.cppm
    do.course.cppm

    do.secretary.cppm
)


source_group("domain-module" FILES ${DOMAIN_MODULES})


target_sources(CourseManagement-system  PRIVATE FILE_SET cxx_modules TYPE CXX_MODULES
    BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        FILES ${DOMAIN_MODULES}
              ${UI_HEADER}
              ${SYSTEM_MODULES}
              ${SERVICE_MODULES}
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
