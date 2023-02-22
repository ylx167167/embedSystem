MESSAGE (STATUS "+++ yuxer-config.cmake")



#===============================================================================
#系统依赖库名
SET (SYS_LIB pthread rt)

#系统头文件目录
SET (SYS_INC_DIR /usr/local/include/)
#include_directories (${SYS_INC_DIR})
MESSAGE (STATUS "The SYS_INC_DIR:" ${SYS_INC_DIR})


#系统库依赖目录
SET (SYS_LIB_DIR /usr/local/lib/)
#link_directories (${SYS_LIB_DIR})
MESSAGE (STATUS "The SYS_LIB_DIR:" ${SYS_LIB_DIR})
#===============================================================================

#设置CMAKE子模块搜索目录
SET (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
MESSAGE (STATUS "准备搜索子模块在:" ${CMAKE_MODULE_PATH})




find_package(PAHOMQTT) # 查找OPENCV4的库依赖信息
if(PAHOMQTT_FOUND)
  include_directories(${PAHOMQTT_INCLUDE_DIRS})
  message(STATUS "INCLUDE_DIRECTORIES PAHOMQTT: ${PAHOMQTT_INCLUDE_DIRS}")
  link_directories(${PAHOMQTT_LIBRARIES_DIRS})
  message(STATUS "LINK_DIRECTORIES PAHOMQTT: ${PAHOMQTT_LIBRARIES_DIRS}")
endif()

find_package(OPENCV) # 查找OPENCV4的库依赖信息
if(OPENCV4_FOUND)
  include_directories(${OPENCV4_INCLUDE_DIRS})
  message(STATUS "INCLUDE_DIRECTORIES OPENCV4: ${OPENCV4_INCLUDE_DIRS}")
  link_directories(${OPENCV4_LIBRARIES_DIRS})
  message(STATUS "LINK_DIRECTORIES OPENCV4: ${OPENCV4_LIBRARIES_DIRS}")
endif()


find_package(HIPE) # 查找CJSON的库依赖信息
if(HIPE_FOUND)
  include_directories(${HIPE_INCLUDE_DIRS})
  message(STATUS "INCLUDE_DIRECTORIES HIPE: ${HIPE_INCLUDE_DIRS}")
endif()

find_package(PARSE) # 查找解析库 JSON XML等


# 本地工作库
ADD_SUBDIRECTORY (workspace)
include_directories(${WORKSPACE_INC})

# ===============================================================================
# 添加 公共工具库
add_subdirectory(utils)
include_directories(${UTILS_INC})

# ===============================================================================
# 添加 核心组件
add_subdirectory(core)
include_directories(${CORE_INC})

#链接时的依赖列表，被依赖的要放后面！！！
list(APPEND TARGET_LINK_OBJ ${CORE_OBJ})
list(APPEND TARGET_LINK_OBJ ${UTILS_OBJ})
list (APPEND TARGET_LINK_OBJ ${WORKSPACE_OBJ})


#===============================================================================
#调试用，用于查看头文件是否包含
#获取include目录
get_property(INC_DIRS DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
message (STATUS "INC_DIRS :" ${INC_DIRS})
message(STATUS "The SYS_LIB:" ${SYS_LIB})
message(STATUS "The PAHOMQTT_LINK_LIBS:" ${PAHOMQTT_LIBRARIES_DIRS})
message(STATUS "The OPENCV4_LINK_LIBS:" ${OPENCV4_LIBRARIES_DIRS})
#===============================================================================
#构建APP

ADD_EXECUTABLE (app main.cpp)

target_link_libraries (	app
						${TARGET_LINK_OBJ}
						${OPENCV4_LINK_LIBS}
            ${PAHOMQTT_LINK_LIBS}
						${SYS_LIB})

#输出资源 =======================================================================
install(TARGETS app DESTINATION bin)

#加载资源 =======================================================================

#第三方库 =======================================================================

# if(PAHOMQTT_FOUND)
#   foreach(lib ${PAHOMQTT_INSTALL_LIBS})
#     install(FILES ${PAHOMQTT_LIBRARIES_DIRS}/${lib} DESTINATION lib)
#   endforeach(lib)
# endif()
# if(OPENCV4_FOUND)
#   foreach(lib ${OPENCV4_INSTALL_LIBS})
#     install(FILES ${OPENCV4_LIBRARIES_DIRS}/${lib} DESTINATION lib)
#   endforeach(lib)
# endif()


MESSAGE (STATUS "--- yuxer-config.cmake")