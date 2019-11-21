cmake_minimum_required(VERSION 3.14)
project(OpenSceneGraphDemo)

set(CMAKE_CXX_STANDARD 14)

# brew install open-scene-graph
set(OSG_H /usr/local/Cellar/open-scene-graph/3.6.3/include)
include_directories(${OSG_H})

set(OSG_LINK /usr/local/Cellar/open-scene-graph/3.6.3/lib/libosg.dylib)
set(OPEN_THREADS_LINK /usr/local/Cellar/open-scene-graph/3.6.3/lib/libOpenThreads.dylib)
set(OSG_DB_LINK /usr/local/Cellar/open-scene-graph/3.6.3/lib/libosgDB.dylib)
set(OSG_TEXT_LINK /usr/local/Cellar/open-scene-graph/3.6.3/lib/libosgText.dylib)
set(OSG_VIEWER_LINK /usr/local/Cellar/open-scene-graph/3.6.3/lib/libosgViewer.dylib)
set(OSG_UTIL_LINK /usr/local/Cellar/open-scene-graph/3.6.3/lib/libosgUtil.dylib)
set(OSG_GA_LINK /usr/local/Cellar/open-scene-graph/3.6.3/lib/libosgGA.dylib)
link_libraries(${OSG_LINK} ${OPEN_THREADS_LINK} ${OSG_DB_LINK} ${OSG_TEXT_LINK} ${OSG_VIEWER_LINK} ${OSG_UTIL_LINK} ${OSG_GA_LINK})

set(SOURCE_FILES
        demo/Simple/SimpleMain.cpp demo/Simple/SimpleSG.cpp
        demo/StateAttribute/StateMain.cpp demo/StateAttribute/StateSG.cpp
        demo/TextureMapping/TextureMappingMain.cpp demo/TextureMapping/TextureMappingSG.cpp
        demo/Lighting/LightingMain.cpp demo/Lighting/LightingSG.cpp
        demo/Text/TextMain.cpp demo/Text/TextSG.cpp
        demo/Viewer/ViewerMain.cpp demo/Viewer/ViewerMain_2.cpp
        demo/Callback/CallbackMain.cpp
        demo/FindNode/FindNodeMain.cpp
        demo/Picking/PickingMain.cpp
        )
add_executable(OpenSceneGraphDemo ${SOURCE_FILES})
