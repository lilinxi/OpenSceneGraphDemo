#include <osg/Geode>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/StateSet>
#include <osg/StateAttribute>
#include <osg/ShadeModel>
#include <osg/CullFace>
#include <osg/PolygonMode>
#include <osg/LineWidth>
#include <osgDB/WriteFile>
#include <iostream>

using namespace std;

osg::ref_ptr<osg::Drawable> createDrawable_2_2();

osg::ref_ptr<osg::Node> createSceneGraph_2_2();

int main() {
    osg::ref_ptr<osg::Node> root = createSceneGraph_2_2();
    if (!root.valid()) {
        osg::notify(osg::FATAL) << "Failed in createSceneGraph()." << endl;
    }
    bool result = osgDB::writeNodeFile(*(root.get()), "2_2.osg");
    if (!result) {
        osg::notify(osg::FATAL) << "Failed in osgDB::writeNode()." << endl;
    }
    return 0;
}


osg::ref_ptr<osg::Node> createSceneGraph_2_2() {
    // 创建 Group 根节点。
    osg::ref_ptr<osg::Group> root = new osg::Group;
    {
        // 在根节点的 StateSet 中禁止光照。
        // 使用 PROTECTED 以保证这一修改不会被 osgviewer 覆盖。
        osg::StateSet *state = root->getOrCreateStateSet();
        state->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
    }
    // 创建 Geode 叶节点并关联 Drawable。
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(createDrawable_2_2().get());
    osg::Matrix m;
    {
        // 左上角:使用缺省属性渲染几何体。
        osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
        m.makeTranslate(-2.f, 0.f, 2.f);
        mt->setMatrix(m);
        root->addChild(mt.get());
        mt->addChild(geode.get());
    }
    {
        // 右上角:设置着色模式为 FLAT(单色)。
        osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
        m.makeTranslate(2.f, 0.f, 2.f);
        mt->setMatrix(m);
        root->addChild(mt.get());
        mt->addChild(geode.get());
        osg::StateSet *state = mt->getOrCreateStateSet();
        osg::ShadeModel *sm = new osg::ShadeModel();
        sm->setMode(osg::ShadeModel::FLAT);
        state->setAttribute(sm);
    }
    {
        // 左下角:开启背面剔除。
        // (createDrawable()返回的两个四边形有着相反的顶点时针顺序，因此无论视点的位置如何，都会有一个背面存在。)
        osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
        m.makeTranslate(-2.f, 0.f, -2.f);
        mt->setMatrix(m);
        root->addChild(mt.get());
        mt->addChild(geode.get());
        osg::StateSet *state = mt->getOrCreateStateSet();
        osg::CullFace *cf = new osg::CullFace(); // 缺省值为 BACK
        state->setAttributeAndModes(cf);
    }
    {
        // 右下角:设置多边形填充模式为 LINE(线框)。
        osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
        m.makeTranslate(2.f, 0.f, -2.f);
        mt->setMatrix(m);
        root->addChild(mt.get());
        mt->addChild(geode.get());
        osg::StateSet *state = mt->getOrCreateStateSet();
        osg::PolygonMode *pm = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
        state->setAttributeAndModes(pm);
        // 同时还设置线宽为 3。
        osg::LineWidth *lw = new osg::LineWidth(3.f);
        state->setAttribute(lw);
    }
    return root.get();
}

osg::ref_ptr<osg::Drawable> createDrawable_2_2() {
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
    geom->setVertexArray(v.get());
    v->push_back(osg::Vec3(-1.15f, -.5f, -.85f));
    v->push_back(osg::Vec3(.85f, -.5f, -.85f));
    v->push_back(osg::Vec3(.85f, -.5f, 1.15f));
    v->push_back(osg::Vec3(-1.15f, -.5f, 1.15f));
    v->push_back(osg::Vec3(-.85f, .5f, -1.15f));
    v->push_back(osg::Vec3(-.85f, .5f, .85f));
    v->push_back(osg::Vec3(1.15f, .5f, .85f));
    v->push_back(osg::Vec3(1.15f, .5f, -1.15f));

    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array;
    geom->setColorArray(c.get());
    geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    c->push_back(osg::Vec4(1.f, 0.33f, 0.f, 1.f));
    c->push_back(osg::Vec4(0.f, 1.f, 0.f, 1.f));
    c->push_back(osg::Vec4(0.f, 0.33f, 1.f, 1.f));
    c->push_back(osg::Vec4(1.f, 1.f, 1.f, 1.f));
    c->push_back(osg::Vec4(1.f, 0.f, 0.f, 1.f));
    c->push_back(osg::Vec4(1.f, .66f, 1.f, 1.f));
    c->push_back(osg::Vec4(0.f, 0.f, 1.f, 1.f));
    c->push_back(osg::Vec4(0.f, .66f, 0.f, 1.f));

    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 8));

    return geom.get();
}