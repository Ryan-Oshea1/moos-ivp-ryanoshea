#--------------------------------------------------------
# The CMakeLists.txt for:                       pPointAssign
# Author(s):                              Ryan OShea
#--------------------------------------------------------

SET(SRC
  PointAssign.cpp
  PointAssign_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pPointAssign ${SRC})

TARGET_LINK_LIBRARIES(pPointAssign
   ${MOOS_LIBRARIES}
   mbutil
   geometry
   m
   pthread)

