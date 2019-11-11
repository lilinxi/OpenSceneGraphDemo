// Texture Mapping Example, Texture mapped tree, blending, alpha test

#include <osg/ref_ptr>
#include <osgDB/Registry>
#include <osgDB/WriteFile>
#include <osg/Notify>
#include <iostream>

using std::endl;

osg::ref_ptr<osg::Node> createSceneGraph_3();

int main(int argc, char **argv) {
    osg::ref_ptr<osg::Node> root = createSceneGraph_3();
    if (!root.valid()) {
        osg::notify(osg::FATAL) << "Failed in createSceneGraph()." << endl;
        return 1;
    }

    std::string out("TextureMapping.osg");
    if (!(osgDB::writeNodeFile(*(root.get()), out))) {
        osg::notify(osg::FATAL) << "Failed in osgDB::writeNodeFile()." << endl;
        return 1;
    }

    osg::notify(osg::ALWAYS) << "Successfully wrote \"" << out << "\". Execute \"osgviewer " << out << "\" to view."
                             << endl;
    return 0;
}
