#--------------------------------------------------------
# The CMakeLists.txt for:                       pCommunicationAngle
# Author(s):                              Ryan OShea
#--------------------------------------------------------

SET(SRC
  CommunicationAngle.cpp
  CommunicationAngle_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pCommunicationAngle ${SRC})

TARGET_LINK_LIBRARIES(pCommunicationAngle
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

