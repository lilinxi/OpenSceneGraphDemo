// Text Example, NodeKits and using the osgText library

#include <osg/Geode>
#include <osg/Geometry>
#include <osgText/Font>
#include <osgText/Text>


osg::ref_ptr<osg::Drawable> createBase_5() {
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
    geom->setVertexArray(v.get());
    v->push_back(osg::Vec3(-1.f, 0.f, -1.f));
    v->push_back(osg::Vec3(1.f, 0.f, -1.f));
    v->push_back(osg::Vec3(1.f, 0.f, 1.f));
    v->push_back(osg::Vec3(-1.f, 0.f, 1.f));

    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array;
    geom->setColorArray(c.get());
    geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    c->push_back(osg::Vec4(1.f, 0.f, 0.f, 1.f));
    c->push_back(osg::Vec4(0.f, 1.f, 0.f, 1.f));
    c->push_back(osg::Vec4(0.f, 0.f, 1.f, 1.f));
    c->push_back(osg::Vec4(1.f, 1.f, 1.f, 1.f));

    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array;
    geom->setNormalArray(n.get());
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
    n->push_back(osg::Vec3(0.f, -1.f, 0.f));

    geom->addPrimitiveSet(
            new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

    return geom.get();
}

osg::ref_ptr<osg::Node> createSceneGraph_5() {
    // Create the root (and only) node.
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    geode->addDrawable(createBase_5().get());

    // If the given file could not be found, the path part will be stripped and the file will be searched again in the OS specific directories.
    // 很智能
    osg::ref_ptr<osgText::Font> font = osgText::readFontFile(
            "/Users/limengfan/CLionProjects/OpenSceneGraphDemo/demo/Text/arial.ttf");

    osg::Vec4 white(1.f, 1.f, 1.f, 1.f);
    {
        osg::ref_ptr<osgText::Text> text = new osgText::Text;
        text->setFont(font.get());
        text->setColor(white);
        text->setCharacterSize(.15f);
        // 指定字符在屏幕上显示的大小，而不是物体的真实坐标。
        // text->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
        text->setPosition(osg::Vec3(1.f, 0.f, 1.f));
        text->setAxisAlignment(osgText::Text::SCREEN); // 文字方向：在屏幕上
        text->setText("Top-right");
        geode->addDrawable(text.get());
    }
    {
        osg::ref_ptr<osgText::Text> text = new osgText::Text;
        text->setFont(font.get());
        text->setColor(white);
        text->setCharacterSize(.15f);
        text->setPosition(osg::Vec3(-1.f, 0.f, 1.f));
        text->setAxisAlignment(osgText::Text::SCREEN); // 文字方向：在屏幕上
        text->setText("Top-left");
        geode->addDrawable(text.get());
    }
    {
        osg::ref_ptr<osgText::Text> text = new osgText::Text;
        text->setFont(font.get());
        text->setFontResolution(128, 128); // 设置字体的分辨率，默认32X32
        text->setColor(osg::Vec4(0.f, 0.f, 1.f, 1.f));
        text->setCharacterSize(.4f);
        text->setPosition(osg::Vec3(0.f, 0.f, -1.04f));
        text->setAxisAlignment(osgText::Text::XZ_PLANE); // 文字方向：在XZ平面上
        text->setAlignment(osgText::Text::CENTER_TOP); // 文字对齐方式
        text->setText("Hello OSG World");
        geode->addDrawable(text.get());
    }

    return geode.get();
}
