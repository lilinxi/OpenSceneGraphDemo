// Simple Example, Basic Geode and Geometry class usage

#include <osg/ref_ptr>
#include <osgDB/Registry>
#include <osgDB/WriteFile>
#include <osg/Notify>
#include <iostream>

using std::endl;

osg::ref_ptr<osg::Node> createSceneGraph_1();

int main_1(int argc, char **argv) {
    osg::ref_ptr<osg::Node> root = createSceneGraph_1();
    if (!root.valid()) {
        osg::notify(osg::FATAL) << "Failed in createSceneGraph()." << endl;
        return 1;
    }

    std::string out("Simple.osg");
    if (!(osgDB::writeNodeFile(*(root.get()), out))) {
        osg::notify(osg::FATAL) << "Failed in osgDB::writeNodeFile()." << endl;
        return 1;
    }

    osg::notify(osg::ALWAYS) << "Successfully wrote \"" << out << "\". Execute \"osgviewer " << out << "\" to view."
                             << endl;
    return 0;
}
