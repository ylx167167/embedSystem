CMAKE_MINIMUM_REQUIRED (VERSION 3.5)
# CMAKE 最小版本
MESSAGE (STATUS "+++ yuxer-config.cmake")

#设置版本号   
SET (MAJOR_VERSION 1)

#选择比赛类型
option(EmbedSystem_BS      "challengeBS"  ON)

#x选择编译系统

option(BUILD_DEBUG    "build debug"    OFF)


option(GCC_DEBUG    "GCC"    ON)

#选择编译类型
if(GCC_DEBUG)
	# set (CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
	# set (CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
	set (CMAKE_C_COMPILER gcc)
	set (CMAKE_CXX_COMPILER g++)
	
	add_definitions(-std=c++11 -Wall -fPIC -lpthread)
	add_definitions (-DPJ_AUTOCONF=1 -DPJ_IS_BIG_ENDIAN=0 -DPJ_IS_LITTLE_ENDIAN=1)

else()
message(FATAL_ERROR "未选择模式")
endif()


#调试运行即可
if(BUILD_DEBUG)
	set (BUILD_VERSION_TYPE Debug)
	#编译时加入DEBUG信息
	add_definitions(-g -DSYSTEM_DEBUG)	# 调试包不优化
	message(STATUS "### 编译在线调试代码 ###")
endif()



# 项目名称 ##################################################################
project (app)


# 检查是否需要更新版本
function(CheckBuildVersion CHECK_VERSION_OUTPOUT)
	EXECUTE_PROCESS (	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
										COMMAND git status -s --ignore-submodules
										#取返回值
										OUTPUT_VARIABLE CHECK_RET	)

	SET(${CHECK_VERSION_OUTPOUT} ${CHECK_RET} PARENT_SCOPE)
endfunction()

#获取当前时间
FUNCTION (GetCurrentTime CURRENT_TIME)
	#执行shell指令获取时间
	EXECUTE_PROCESS (
		#前一条命令的标准输出以管道的方式成为下一条命令的标准输入
		#通过shell获取当前时间，结果带有回车换行
		COMMAND date +%y%m%d%H%M
		#回车换行替换成空格
		COMMAND tr -d "\n"
		# COMMAND tr "\n" ""
		#取返回值
		OUTPUT_VARIABLE CUR_TIME	)
	#COMMAND ${CUR_TIME} | awk '{printf("%s", $0)}'

	#设置当前时间，PARENT_SCOPE 指定作用域为调用者(父进程)
	SET(${CURRENT_TIME} 
	${CUR_TIME} 
	PARENT_SCOPE)
ENDFUNCTION()



function(CheckRunEnvir )
	#git
	execute_process( COMMAND git --version OUTPUT_VARIABLE GIT_VERSION )
	message(STATUS "GIT_VERSION:${GIT_VERSION}")
	if(NOT GIT_VERSION)
		execute_process( COMMAND sudo apt-get install git	)
	endif()

	#autoconf
	execute_process(	COMMAND autoconf -V OUTPUT_VARIABLE AUTOCONF_VERSION )
	message(STATUS "AUTOCONF_VERSION:${AUTOCONF_VERSION}")
	if(NOT AUTOCONF_VERSION)
		execute_process( COMMAND sudo apt-get install autoconf )
	endif()
endfunction()
#检查运行环境
CheckRunEnvir()

#检查 git 工作区与版本库是否有差异
CheckBuildVersion(CHECK_VERSION_OUTPOUT)
# MESSAGE (STATUS "====> CHECK_VERSION_OUTPOUT:" ${CHECK_VERSION_OUTPOUT} "<====")
if("${CHECK_VERSION_OUTPOUT}" STREQUAL "")
	message(STATUS "不更新 BUILD VERSION")

else()
	message(STATUS "更新 BUILD VERSION")

	#获取当前时间,作为版本号的最后一部分
	GetCurrentTime(CURRENT_TIME)
	SET (BUILD_VERSION ${BUILD_VERSION_TYPE}_${CURRENT_TIME})
	MESSAGE (STATUS "====> CURRENT_TIME:" ${CURRENT_TIME} "<====")
endif()

# #将xxx-config.cmake里的版本号转换为程序版本号
# configure_file(	"${PROJECT_SOURCE_DIR}/versionConfig.h.in"
# 					"${PROJECT_SOURCE_DIR}/versionConfig.h")


#工程输出根目录
set (PROJECT_OUPUT_ROOT ${PROJECT_SOURCE_DIR}/output)


#设置CMAKE子模块搜索目录
set (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
message (STATUS "cmake search dir:" ${CMAKE_MODULE_PATH})

if (EmbedSystem_BS)
	message (STATUS ">>>>> 编译 EmbedSystem <<<<<")
	#指定调用某个cmake文件 一定要用xxx_DIR
	set (EmbedSystem_DIR ${PROJECT_SOURCE_DIR}/cmake)
	message (STATUS "<<<<<  >>>>>")
    find_package (EMBEDSYSTEM)
	message (STATUS "<<<<< 编译 EmbedSystem >>>>>")
endif()

message (STATUS "PROJECT_OUTPUT_APP:" ${PROJECT_OUTPUT_APP})
