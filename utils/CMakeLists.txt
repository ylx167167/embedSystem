MESSAGE (STATUS "+++ utils +++")

#设置utils的根目录
SET (UTILS_DIR ${PROJECT_SOURCE_DIR}/utils)

#包含头文件目录
include_directories(${UTILS_DIR})
include_directories(${PROJECT_SOURCE_DIR}/core)


#添加 jsoncpp 自定义接口
ADD_SUBDIRECTORY (my_string)
# list (APPEND UTILS_INC ${UTILS_DIR}/json)
list (APPEND UTILS_OBJ utils_my_string)

#添加 hash 公共接口
ADD_SUBDIRECTORY (hash)
list (APPEND UTILS_OBJ utils_hash)

# #添加 udp_pieces 自定义接口
ADD_SUBDIRECTORY (udp_piece)
list (APPEND UTILS_OBJ utils_udp_pieces)

#设置为上级作用域可用
SET (UTILS_INC ${UTILS_DIR} PARENT_SCOPE)
SET (UTILS_OBJ ${UTILS_OBJ} PARENT_SCOPE)

MESSAGE (STATUS "--- utils ---")
