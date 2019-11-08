#include <osg/Geode>
#include <osg/Geometry>
#include <osg/ref_ptr>
#include <osg/Notify>
#include <osgDB/WriteFile>
#include <iostream>

using namespace std;

osg::ref_ptr<osg::Node> createSceneGraph_2_1();

int main_2_1() {
    osg::ref_ptr<osg::Node> root = createSceneGraph_2_1();
    if (!root.valid()) {
        osg::notify(osg::FATAL) << "Failed in createSceneGraph()." << endl;
    }
    bool result = osgDB::writeNodeFile(*(root.get()), "2_1.osg");
    if (!result) {
        osg::notify(osg::FATAL) << "Failed in osgDB::writeNode()." << endl;
    }
    return 0;
}


osg::ref_ptr<osg::Node> createSceneGraph_2_1() {
    // 创建一个用于保存几何信息的对象
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
    // 创建四个顶点的数组
    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
    geom->setVertexArray(v.get());
    v->push_back(osg::Vec3(-1.f, 0.f, -1.f));
    v->push_back(osg::Vec3(1.f, 0.f, -1.f));
    v->push_back(osg::Vec3(1.f, 0.f, 1.f));
    v->push_back(osg::Vec3(-1.f, 0.f, 1.f));
    // 创建四种颜色的数组
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array;
    geom->setColorArray(c.get());
    geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    c->push_back(osg::Vec4(1.f, 0.f, 0.f, 1.f));
    c->push_back(osg::Vec4(0.f, 1.f, 0.f, 1.f));
    c->push_back(osg::Vec4(0.f, 0.f, 1.f, 1.f));
    c->push_back(osg::Vec4(1.f, 1.f, 1.f, 1.f));
    // 为唯一的法线创建一个数组
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array;
    geom->setNormalArray(n.get());
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
    n->push_back(osg::Vec3(0.f, -1.f, 0.f));
    // 由保存的数据绘制四个顶点的多边形
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
    // 向 Geode 类添加几何体(Drawable)并返回 Geode
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(geom.get());
    return geode.get();
}
