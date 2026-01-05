cmake_minimum_required(VERSION 4.1.1)

#https://github.com/Kitware/CMake/blob/v4.1.1/Help/dev/experimental.rst

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
set(CMAKE_CXX_MODULE_STD ON)

project(Base LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 PostgreSQL 和 pqxx 库
find_package(PostgreSQL REQUIRED)

# 在 Arch/Manjaro 上，libpqxx 可能没有提供 CMake 包，所以手动查找
find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

if(NOT PQXX_LIB OR NOT PQ_LIB)
    message(WARNING "找不到 pqxx 或 pq 库，尝试使用 pkg-config...")
    find_package(PkgConfig)
    pkg_check_modules(PQXX REQUIRED libpqxx)
endif()

# set(MODULE_LIB_DIR1 /opt/utils/modules)  # 模块库的目录的设置
add_executable(Base
    #math.cpp                              # 模块实现单元
    main.cpp
    Database.h

)

set(MODULE_INTERFACE_FILES
    #${MODULE_LIB_DIR1}/utils.cppm         #添加模块库的模块接口单元
    #math.cppm                             # 模块接口单元
)

source_group("Module Interfaces" FILES ${MODULE_INTERFACE_FILES})
target_sources(Base  PRIVATE FILE_SET cxx_modules TYPE CXX_MODULES
    BASE_DIRS                              # 配置BASE_DIRS，用于组织构建输出目录，以便构建不同目录中(同名的)模块文件.
        #${MODULE_LIB_DIR1}
        ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${MODULE_INTERFACE_FILES}
)

target_compile_features(Base PRIVATE cxx_std_23)

# 包含目录
target_include_directories(Base PRIVATE
    /usr/include
)

# 链接库
if(PQXX_LIB AND PQ_LIB)
    target_link_libraries(Base PRIVATE ${PQXX_LIB} ${PQ_LIB})
elseif(PQXX_FOUND)
    target_include_directories(Base PRIVATE ${PQXX_INCLUDE_DIRS})
    target_link_libraries(Base PRIVATE ${PQXX_LIBRARIES} PostgreSQL::PostgreSQL)
else()
    # 如果都找不到，尝试直接链接
    target_link_libraries(Base PRIVATE pqxx pq)
endif()

# 添加编译器标志（可选）
target_compile_options(Base PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
