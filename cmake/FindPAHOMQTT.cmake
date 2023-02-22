MESSAGE(STATUS "+++ FindPAHOMQTT.cmake")

if(GCC_DEBUG)
  SET (PAHOMQTT_INCLUDE_DIRS	${PROJECT_SOURCE_DIR}/third/pahoMqtt/include)
  SET (PAHOMQTT_LIBRARIES_DIRS	  ${PROJECT_SOURCE_DIR}/third/pahoMqtt/lib)
endif()

set(PAHOMQTT_LINK_LIBS 
    paho-mqtt3a
    paho-mqtt3as
    paho-mqtt3c
    paho-mqtt3cs
    paho-mqttpp3
)
set(PAHOMQTT_INSTALL_LIBS 
    libpaho-mqtt3a.so
    libpaho-mqtt3as.so
    libpaho-mqtt3c.so
    libpaho-mqtt3cs.so
    libpaho-mqttpp3.so
)

SET (PAHOMQTT_FOUND TRUE)

MESSAGE("PAHOMQTT_INCLUDE_DIRS" ${PAHOMQTT_INCLUDE_DIRS})
MESSAGE("PAHOMQTT_LIBRARIES_DIRS" ${PAHOMQTT_LIBRARIES_DIRS})

MESSAGE(STATUS "--- FindPAHOMQTT.cmake")
