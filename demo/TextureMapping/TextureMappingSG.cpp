// Texture Mapping Example, Texture mapped tree, blending, alpha test

#include <osgDB/ReadFile>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/StateSet>
#include <osg/StateAttribute>
#include <osg/Texture2D>
#include <osg/BlendFunc>
#include <osg/AlphaFunc>
#include <osg/Notify>
#include <string>

osg::ref_ptr<osg::Node> createGeodes_3() {
    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
    v->push_back(osg::Vec3(0.f, 0.f, 0.f)); //0
    v->push_back(osg::Vec3(0.f, 0.f, 4.f));
    v->push_back(osg::Vec3(-1.f, 0.f, 0.f));
    v->push_back(osg::Vec3(1.f, 0.f, 0.f)); //3
    v->push_back(osg::Vec3(1.f, 0.f, 4.f));
    v->push_back(osg::Vec3(-1.f, 0.f, 4.f));
    v->push_back(osg::Vec3(0.f, -1.f, 0.f)); //6
    v->push_back(osg::Vec3(0.f, 1.f, 0.f));
    v->push_back(osg::Vec3(0.f, 1.f, 4.f));
    v->push_back(osg::Vec3(0.f, -1.f, 4.f)); //9

    osg::ref_ptr<osg::Vec2Array> tc = new osg::Vec2Array;
    tc->push_back(osg::Vec2(.5f, 0.f));
    tc->push_back(osg::Vec2(.5f, 1.f));
    tc->push_back(osg::Vec2(0.f, 0.f));
    tc->push_back(osg::Vec2(1.f, 0.f));
    tc->push_back(osg::Vec2(1.f, 1.f));
    tc->push_back(osg::Vec2(0.f, 1.f));
    tc->push_back(osg::Vec2(0.f, 0.f));
    tc->push_back(osg::Vec2(1.f, 0.f));
    tc->push_back(osg::Vec2(1.f, 1.f));
    tc->push_back(osg::Vec2(0.f, 1.f));

    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array;
    c->push_back(osg::Vec4(1.f, 1.f, 1.f, 1.f));

    osg::ref_ptr<osg::Group> grp = new osg::Group;
    {
        osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
        geom->setVertexArray(v.get());
        geom->setTexCoordArray(0, tc.get());
        geom->setColorArray(c.get());
        geom->setColorBinding(osg::Geometry::BIND_OVERALL);

        GLushort indices[] = {2, 0, 1, 5};
        geom->addPrimitiveSet(new osg::DrawElementsUShort(
                osg::PrimitiveSet::QUADS, 4, indices));

        osg::ref_ptr<osg::Geode> geode = new osg::Geode;
        geode->addDrawable(geom.get());
        grp->addChild(geode.get());
    }
    {
        osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
        geom->setVertexArray(v.get());
        geom->setTexCoordArray(0, tc.get());
        geom->setColorArray(c.get());
        geom->setColorBinding(osg::Geometry::BIND_OVERALL);

        GLushort indices[] = {2, 3, 4, 5};
        geom->addPrimitiveSet(new osg::DrawElementsUShort(
                osg::PrimitiveSet::QUADS, 4, indices));

        osg::ref_ptr<osg::Geode> geode = new osg::Geode;
        geode->addDrawable(geom.get());
        grp->addChild(geode.get());
    }
    {
        osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
        geom->setVertexArray(v.get());
        geom->setTexCoordArray(0, tc.get());
        geom->setColorArray(c.get());
        geom->setColorBinding(osg::Geometry::BIND_OVERALL);

        GLushort indices[] = {6, 0, 1, 9};
        geom->addPrimitiveSet(new osg::DrawElementsUShort(
                osg::PrimitiveSet::QUADS, 4, indices));

        osg::ref_ptr<osg::Geode> geode = new osg::Geode;
        geode->addDrawable(geom.get());
        grp->addChild(geode.get());
    }
    {
        osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
        geom->setVertexArray(v.get());
        geom->setTexCoordArray(0, tc.get());
        geom->setColorArray(c.get());
        geom->setColorBinding(osg::Geometry::BIND_OVERALL);

        GLushort indices[] = {0, 7, 8, 1};
        geom->addPrimitiveSet(new osg::DrawElementsUShort(
                osg::PrimitiveSet::QUADS, 4, indices));

        osg::ref_ptr<osg::Geode> geode = new osg::Geode;
        geode->addDrawable(geom.get());
        grp->addChild(geode.get());
    }

    return grp.get();
}

osg::ref_ptr<osg::Node> createSceneGraph_3() {
    osg::ref_ptr<osg::Node> node = createGeodes_3();

    osg::StateSet *state = node->getOrCreateStateSet();
    state->setMode(GL_LIGHTING, osg::StateAttribute::OFF |
                                osg::StateAttribute::PROTECTED);
    state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

    // Load the texture image
    //   Image courtesy Virtual Terrain Project (http://vterrain.org/)
    std::string fileName(
            "/Users/limengfan/CLionProjects/OpenSceneGraphDemo/demo/TextureMapping/Picea_pungens__blue_spruce15_256.png");
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile(fileName);
    if (!image.valid()) {
        osg::notify(osg::FATAL) << "Unable to load data file. Exiting." << std::endl;
        return nullptr;
    }

    // Set the image in a Texture2D object
    osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D;
    tex->setImage(image.get());
    // After creating the OpenGL texture object, release the
    //   internal ref_ptr<Image> (to delete the Image).
    // 大量使用纹理贴图的程序，往往需要实现更紧凑的内存管理。
    //  Image 类继承 自 Referenced 类，而 Texture2D 内部保存了一个指向 Image 的 ref_ptr<>指针。
    //  在第一次渲染时，OSG 创建了用于保存图像数据的 OpenGL 纹理对象，其结果是产生了两个纹理图像的副本：
    //  一个是 Image 对象，另一个由 OpenGL 拥有。
    //  简单的单环境(single-context)场景渲染中，你可以通过设置 Texture2D 解除对 Image 的引用来降低内存损耗。
    //  如果当前引用 Image 对象的只有 Texture2D 对象，那么 OSG 将释放 Image 及其内存空间。
    tex->setUnRefImageDataAfterApply(true);
    // Texture Modes：
    //  - GL_TEXTURE_1D
    //  - GL_TEXTURE_2D
    //  - GL_TEXTURE_3D
    //  - GL_TEXTURE_CUBE_MAP
    //  - GL_TEXTURE_RECTANGLE
    state->setTextureAttributeAndModes(0, tex.get());

    // Turn on blending
    // 源和目标混合
    osg::BlendFunc *bf = new osg::BlendFunc(
            osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
    state->setAttributeAndModes(bf);

    // Turn on alpha testing
    // 使用AlphaTest，将透明的像素剔除，只保留非透明的像素。
    osg::AlphaFunc *af = new osg::AlphaFunc(
            osg::AlphaFunc::GREATER, 0.05f);
    state->setAttributeAndModes(af);

    return node.get();
}
