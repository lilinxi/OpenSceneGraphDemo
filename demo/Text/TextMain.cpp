// Text Example, NodeKits and using the osgText library

#include <osg/ref_ptr>
#include <osgDB/Registry>
#include <osgDB/WriteFile>
#include <osg/Notify>
#include <iostream>

using std::endl;

osg::ref_ptr<osg::Node> createSceneGraph_5();

int main_5(int argc, char **argv) {
    osg::ref_ptr<osg::Node> root = createSceneGraph_5();
    if (!root.valid()) {
        osg::notify(osg::FATAL) << "Failed in createSceneGraph()." << endl;
        return 1;
    }

    std::string out("Text.osg");
    if (!(osgDB::writeNodeFile(*(root.get()), out))) {
        osg::notify(osg::FATAL) << "Failed in osgDB::writeNodeFile()." << endl;
        return 1;
    }

    osg::notify(osg::ALWAYS) << "Successfully wrote \"" << out << "\". Execute \"osgviewer " << out << "\" to view."
                             << endl;
    return 0;
}
