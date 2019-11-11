cmake_minimum_required(VERSION 3.14)
project(OpenSceneGraphDemo)

set(CMAKE_CXX_STANDARD 14)

set(OSG_H /usr/local/Cellar/open-scene-graph/3.6.3/include)
include_directories(${OSG_H})

set(OSG_LINK /usr/local/Cellar/open-scene-graph/3.6.3/lib/libosg.dylib)
set(OPEN_THREADS_LINK /usr/local/Cellar/open-scene-graph/3.6.3/lib/libOpenThreads.dylib)
set(OSG_DB_LINK /usr/local/Cellar/open-scene-graph/3.6.3/lib/libosgDB.dylib)
#set(OSG_VIEWER_LINK /usr/local/Cellar/open-scene-graph/3.6.3/lib/libosgViewer.dylib)
#set(OSG_UTIL_LINK /usr/local/Cellar/open-scene-graph/3.6.3/lib/libosgUtil.dylib)
#link_libraries(${OSG_LINK} ${OPEN_THREADS_LINK} ${OSG_DB_LINK} ${OSG_VIEWER_LINK} ${OSG_UTIL_LINK})
link_libraries(${OSG_LINK} ${OPEN_THREADS_LINK} ${OSG_DB_LINK})

set(SOURCE_FILES
        demo/Simple/SimpleMain.cpp demo/Simple/SimpleSG.cpp
        demo/State/StateMain.cpp demo/State/StateSG.cpp
        demo/TextureMapping/TextureMappingMain.cpp demo/TextureMapping/TextureMappingSG.cpp
        )
add_executable(OpenSceneGraphDemo ${SOURCE_FILES})