// Lighting Example, Basic light and material control

#include <osg/ref_ptr>
#include <osgDB/Registry>
#include <osgDB/WriteFile>
#include <osg/Notify>
#include <iostream>

using std::endl;

osg::ref_ptr<osg::Node> createSceneGraph_4();

int main_4(int argc, char **argv) {
    osg::ref_ptr<osg::Node> root = createSceneGraph_4();
    if (!root.valid()) {
        osg::notify(osg::FATAL) << "Failed in createSceneGraph()." << endl;
        return 1;
    }

    std::string out("Lighting.osg");
    if (!(osgDB::writeNodeFile(*(root.get()), out))) {
        osg::notify(osg::FATAL) << "Failed in osgDB::writeNodeFile()." << endl;
        return 1;
    }

    osg::notify(osg::ALWAYS) << "Successfully wrote \"" << out << "\". Execute \"osgviewer " << out << "\" to view."
                             << endl;
    return 0;
}
