MESSAGE (STATUS "+++ Core +++")

#在这做模块控制 IF(XXX) module_name ENDIF()

#设置 core 的根目录
SET (CORE_DIR ${PROJECT_SOURCE_DIR}/core)

#添加到头文件搜索路径中
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/core)
include_directories(${PROJECT_SOURCE_DIR}/utils)
include_directories(${PROJECT_SOURCE_DIR}/modules)
include_directories(${PROJECT_SOURCE_DIR}/third/pahoMqtt/include)
include_directories(${PROJECT_SOURCE_DIR}/third/cjson/include)

#添加 log 模块
ADD_SUBDIRECTORY (log)
list (APPEND CORE_OBJ core_log)
#添加 console 模块
ADD_SUBDIRECTORY (console)
list (APPEND CORE_OBJ core_console)
#添加 mqtt 模块
ADD_SUBDIRECTORY (mqttclient)
list (APPEND CORE_OBJ core_mqtt)
#添加 线程 模块
ADD_SUBDIRECTORY (thread)
list (APPEND CORE_OBJ core_thread)
# #添加 加密 模块
# ADD_SUBDIRECTORY (crypto)
# list (APPEND CORE_OBJ core_crypto)
#添加 读取配置资源模块 模块
ADD_SUBDIRECTORY (system)
list (APPEND CORE_OBJ core_system)

#添加 读取配置资源模块 模块
ADD_SUBDIRECTORY (parse)
list (APPEND CORE_OBJ core_parse)

#设置为上级作用域可用
SET (CORE_INC ${CORE_DIR} PARENT_SCOPE)
SET (CORE_OBJ ${CORE_OBJ} PARENT_SCOPE)


MESSAGE (STATUS "--- Core ---")
