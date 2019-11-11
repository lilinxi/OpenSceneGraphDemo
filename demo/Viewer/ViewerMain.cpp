// Viewer Example, A minimal OSG viewer

#include <osgViewer/Viewer>
#include <osgDB/ReadFile>

int main(int, char **) {
    // Create a Viewer.
    osgViewer::Viewer viewer;

    // Load a model and add it to the Viewer.
    viewer.setSceneData(osgDB::readNodeFile("/Users/limengfan/CLionProjects/OpenSceneGraphDemo/demo/Viewer/cow.osg"));
    if (!viewer.getSceneData()) {
        osg::notify(osg::FATAL) << "Unable to load data file. Exiting." << std::endl;
        return 1;
    }

    // TBD. the .net loader.
    // viewer.setSceneData( osgDB::readNodeFile(
    //    "http://www.openscenegraph.org/downloads/data/Earth/model.ive" ) );

    // Display, and main loop.
    return viewer.run();
}
