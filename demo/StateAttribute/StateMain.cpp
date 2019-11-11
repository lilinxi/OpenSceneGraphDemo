// State Example, Modifying state attributes and modes

#include <osg/ref_ptr>
#include <osgDB/Registry>
#include <osgDB/WriteFile>
#include <osg/Notify>
#include <iostream>

using std::endl;

osg::ref_ptr<osg::Node> createSceneGraph_2();

int main_2(int argc, char **argv) {
    osg::ref_ptr<osg::Node> root = createSceneGraph_2();
    if (!root.valid()) {
        osg::notify(osg::FATAL) << "Failed in createSceneGraph()." << endl;
        return 1;
    }

    std::string out("State.osg");
    if (!(osgDB::writeNodeFile(*(root.get()), out))) {
        osg::notify(osg::FATAL) << "Failed in osgDB::writeNodeFile()." << endl;
        return 1;
    }

    osg::notify(osg::ALWAYS) << "Successfully wrote \"" << out << "\". Execute \"osgviewer " << out << "\" to view."
                             << endl;

    return 0;
}
