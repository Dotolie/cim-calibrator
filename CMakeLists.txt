CMAKE_MINIMUM_REQUIRED (VERSION 2.8.11)

PROJECT (calibrator)


ADD_DEFINITIONS(-Wall -O2)

SET(BINDIR  ${CMAKE_SOURCE_DIR}/bin)
SET(AUTHOR_NAME "ywkim")

SET ( PROJECT_VERSION_MAJOR 1 )
SET ( PROJECT_VERSION_MINOR 1 )
CONFIGURE_FILE ( ${CMAKE_SOURCE_DIR}/version.h.in  ${CMAKE_SOURCE_DIR}/inc/version.h )

INCLUDE_DIRECTORIES ( 	
	${CMAKE_SOURCE_DIR}/inc
	${CMAKE_SOURCE_DIR}/include

)

#LINK_DIRECTORIES ( ${CMAKE_SOURCE_DIR}/lib )

FILE(GLOB SRCS
	src/main.cpp
	src/msg.cpp
	src/thread.cpp
	src/taskMgr.cpp
	src/timerMgr.cpp
	src/iotBox.cpp
	src/zmqAdaptor.cpp
	src/consol.cpp
	src/utils.cpp
)

ADD_EXECUTABLE(${PROJECT_NAME} ${SRCS})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread rt crypto stdc++ )

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${BINDIR})

