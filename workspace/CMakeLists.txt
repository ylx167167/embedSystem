# MESSAGE (STATUS "+++ workspace +++")

# include_directories(${PROJECT_SOURCE_DIR})
# SET (WORKSPACE_DIR ${PROJECT_SOURCE_DIR}/workspace)

# AUX_SOURCE_DIRECTORY (. DIR_BASE_SRC)
# ADD_LIBRARY (workspace_modules ${DIR_BASE_SRC})
# list (APPEND WORKSPACE_OBJ workspace_modules)

# #设置为上级作用域可用
# SET (WORKSPACE_INC ${WORKSPACE_DIR} PARENT_SCOPE)
# SET (WORKSPACE_OBJ ${WORKSPACE_OBJ} PARENT_SCOPE)

# MESSAGE (STATUS "--- workspace ---")


MESSAGE (STATUS "+++ workspace +++")


SET (WORKSPACE_DIR ${PROJECT_SOURCE_DIR}/workspace)
#添加 Slam_ 模块
include_directories( "/usr/include/eigen3" )
ADD_SUBDIRECTORY (Slam_)
list (APPEND WORKSPACE_OBJ Work_slam)



#设置为上级作用域可用
SET (WORKSPACE_INC ${WORKSPACE_DIR} PARENT_SCOPE)
SET (WORKSPACE_OBJ ${WORKSPACE_OBJ} PARENT_SCOPE)

MESSAGE (STATUS "--- workspace ---")
