// Lighting Example, Basic light and material control

#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/StateSet>
#include <osg/StateAttribute>
#include <osg/CullFace>
#include <osg/Point>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/Material>
#include <osg/Notify>

osg::ref_ptr<osg::Geode> createLightPoint_4() {
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

    // Don't throw away single-vertex primitives.
    // TODO 为什么 BoundingBox 和 PointSize 影响了光源的显示
    osg::BoundingBox bbox(-.1f, -.1f, -.1f, .1f, .1f, .1f);
    geom->setInitialBound(bbox);

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
    geom->setVertexArray(v.get());
    v->push_back(osg::Vec3(0.f, 0.f, 0.f));

    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array;
    geom->setColorArray(c.get());
    geom->setColorBinding(osg::Geometry::BIND_OVERALL);
    c->push_back(osg::Vec4(1.f, 1.f, 1.f, 1.f));

    geom->addPrimitiveSet(new osg::DrawArrays(GL_POINTS, 0, 1));

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(geom.get());

    osg::StateSet *state = geode->getOrCreateStateSet();
    state->setMode(GL_LIGHTING, osg::StateAttribute::OFF |
                                osg::StateAttribute::PROTECTED);
    osg::ref_ptr<osg::Point> pt = new osg::Point;
    pt->setSize(10.f);
    state->setAttribute(pt.get());

    return geode.get();
}

osg::ref_ptr<osg::Drawable> createPlane_4() {
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
    geom->setVertexArray(v.get());
    int x, y;
    for (y = -10; y <= 10; y++) {
        for (x = -10; x <= 10; x++) {
            v->push_back(osg::Vec3((float) x * .5f, (float) y * .5f, 0.f));
        }
    }

    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array;
    geom->setNormalArray(n.get());
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
    n->push_back(osg::Vec3(0.f, 0.f, 1.f));

    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array;
    geom->setColorArray(c.get());
    geom->setColorBinding(osg::Geometry::BIND_OVERALL);
    c->push_back(osg::Vec4(1.f, 1.f, 1.f, 1.f));

    const int len(21);
    int idx(0);
    int numStrips(len - 1);
    while (numStrips--) {
        GLushort indices[len + len];
        int vert(0);
        while (vert < len) {
            indices[2 * vert + 0] = idx + len;
            indices[2 * vert + 1] = idx;
            vert++;
            idx++;
        }
        geom->addPrimitiveSet(new osg::DrawElementsUShort(
                osg::PrimitiveSet::QUAD_STRIP, len + len, indices));
    }

    return geom.get();
}

osg::ref_ptr<osg::Node> createSceneGraph_4() {
    // Create the root node and set state for the entire sibgraph.
    osg::ref_ptr<osg::Group> root = new osg::Group;
    {
        osg::StateSet *state = root->getOrCreateStateSet();
        state->setMode(GL_LIGHTING, osg::StateAttribute::ON);
        state->setMode(GL_LIGHT0, osg::StateAttribute::ON);
        state->setMode(GL_LIGHT1, osg::StateAttribute::ON);
        // Should be off by default, but osgviewer enables it.
        // state->setMode(GL_COLOR_MATERIAL, osg::StateAttribute::OFF);
    }

    // Represent the two light sources with a shared point.
    // Each light's position is 0,0,0 in object coordinates. The
    //   two parent MatrixTransforms translate the lights, and the
    //   shared point, to their actual positions.
    osg::ref_ptr<osg::Geode> lightPoint = createLightPoint_4();
    {
        osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
        osg::Matrix m;
        m.makeTranslate(osg::Vec3(-3.f, 2.f, 5.f));
        mt->setMatrix(m);

        // Create a mostly red light
        osg::ref_ptr<osg::Light> light = new osg::Light;
        light->setLightNum(0); // 关联光源
        light->setPosition(osg::Vec4(0.f, 0.f, 0.f, 1.f)); // 位置
        light->setAmbient(osg::Vec4(.1f, .1f, .1f, 1.f)); // 环境光颜色
        light->setDiffuse(osg::Vec4(1.f, .5f, .5f, 1.f)); // 散射光颜色
        light->setSpecular(osg::Vec4(1.f, .8f, .8f, 1.f)); // 镜面反射光颜色
        light->setDirection(osg::Vec3(0.f, 0.f, -1.f)); // 方向
        // light->setSpotCutoff(25.f); // 切光角

        osg::ref_ptr<osg::LightSource> ls = new osg::LightSource;
        root->addChild(mt.get());
        mt->addChild(ls.get());
        ls->setLight(light.get());
        ls->addChild(lightPoint.get());
    }
    {
        osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
        osg::Matrix m;
        m.makeTranslate(osg::Vec3(3.f, -2.f, 3.f));
        mt->setMatrix(m);

        // Create a mostly blue light
        osg::ref_ptr<osg::Light> light = new osg::Light;
        light->setLightNum(1);
        light->setPosition(osg::Vec4(0.f, 0.f, 0.f, 1.f));
        light->setDiffuse(osg::Vec4(.5f, .5f, 1.f, 1.f));
        light->setSpecular(osg::Vec4(.8f, .8f, 1.f, 1.f));

        osg::ref_ptr<osg::LightSource> ls = new osg::LightSource;
        root->addChild(mt.get());
        mt->addChild(ls.get());
        ls->setLight(light.get());
        ls->addChild(lightPoint.get());
    }

    // Create a single instance of the lozenge geometry (read from disk).
    // Multiply parent it to six MatrixTransform nodes, each with their
    //   own StateSet to change the material properties of the lozenge.
    osg::ref_ptr<osg::Node> lozenge = osgDB::readNodeFile(
            "/Users/limengfan/CLionProjects/OpenSceneGraphDemo/demo/Lighting/lozenge.osg");
    if (!lozenge.valid()) {
        osg::notify(osg::FATAL) << "Unable to load data file. Exiting." << std::endl;
        return nullptr;
    }
    {
        osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
        osg::Matrix m;
        m.makeTranslate(osg::Vec3(-1.f, -1.f, 1.f));
        mt->setMatrix(m);

        osg::StateSet *state = mt->getOrCreateStateSet();
        osg::ref_ptr<osg::Material> mat = new osg::Material;
        mat->setDiffuse(osg::Material::FRONT,
                        osg::Vec4(0.f, 0.f, 0.f, 1.f)); // 漫反射材质
        mat->setSpecular(osg::Material::FRONT,
                         osg::Vec4(1.f, 1.f, 1.f, 1.f)); // 镜面反射材质
        mat->setShininess(osg::Material::FRONT, 128.f); // 镜面指数
        state->setAttribute(mat.get());

        mt->addChild(lozenge.get());
        root->addChild(mt.get());
    }
    {
        osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
        osg::Matrix m;
        m.makeTranslate(osg::Vec3(1.f, -1.f, 1.f));
        mt->setMatrix(m);

        osg::StateSet *state = mt->getOrCreateStateSet();
        osg::ref_ptr<osg::Material> mat = new osg::Material;
        // Just use the object's primary color for ambient and
        //   diffuse (uses the OpenGL color material feature).
        // 要允许颜色跟踪材质的特性，需要调用 Material::setColorMode()方法。
        // Material 类为之定义了枚举量 AMBIENT，DIFFUSE，SPECULAR，EMISSION， AMBIENT_AND_DIFFUSE 以及 OFF。
        // 根据颜色跟踪模式的取值不同，Material 类会自动允许或禁止 GL_COLOR_MATERIAL。
        mat->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
        state->setAttribute(mat.get());

        mt->addChild(lozenge.get());
        root->addChild(mt.get());
    }
    {
        osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
        osg::Matrix m;
        m.makeTranslate(osg::Vec3(-1.f, 0.f, 1.f));
        mt->setMatrix(m);

        osg::StateSet *state = mt->getOrCreateStateSet();
        osg::ref_ptr<osg::Material> mat = new osg::Material;
        mat->setDiffuse(osg::Material::FRONT,
                        osg::Vec4(.4f, .3f, 0.f, 1.f));
        mat->setSpecular(osg::Material::FRONT,
                         osg::Vec4(.8f, .8f, .1f, 1.f));
        mat->setShininess(osg::Material::FRONT, 20.f);
        state->setAttribute(mat.get());

        mt->addChild(lozenge.get());
        root->addChild(mt.get());
    }
    {
        osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
        osg::Matrix m;
        m.makeTranslate(osg::Vec3(1.f, 0.f, 1.f));
        mt->setMatrix(m);

        osg::StateSet *state = mt->getOrCreateStateSet();
        osg::ref_ptr<osg::Material> mat = new osg::Material;
        mat->setDiffuse(osg::Material::FRONT,
                        osg::Vec4(.1f, .2f, .5f, 1.f));
        mat->setSpecular(osg::Material::FRONT,
                         osg::Vec4(.9f, .9f, 1.f, 1.f));
        mat->setShininess(osg::Material::FRONT, 10.f);
        state->setAttribute(mat.get());

        mt->addChild(lozenge.get());
        root->addChild(mt.get());
    }
    {
        osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
        osg::Matrix m;
        m.makeTranslate(osg::Vec3(-1.f, 1.f, 1.f));
        mt->setMatrix(m);

        osg::StateSet *state = mt->getOrCreateStateSet();
        osg::ref_ptr<osg::Material> mat = new osg::Material;
        mat->setDiffuse(osg::Material::FRONT,
                        osg::Vec4(.2f, .9f, .9f, 1.f));
        mat->setSpecular(osg::Material::FRONT,
                         osg::Vec4(1.f, 1.f, 1.f, 1.f));
        mat->setShininess(osg::Material::FRONT, 96.f);
        state->setAttribute(mat.get());

        mt->addChild(lozenge.get());
        root->addChild(mt.get());
    }
    {
        osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
        osg::Matrix m;
        m.makeTranslate(osg::Vec3(1.f, 1.f, 1.f));
        mt->setMatrix(m);

        osg::StateSet *state = mt->getOrCreateStateSet();
        osg::ref_ptr<osg::Material> mat = new osg::Material;
        mat->setDiffuse(osg::Material::FRONT,
                        osg::Vec4(1.f, 1.f, 1.f, 1.f));
        mat->setSpecular(osg::Material::FRONT,
                         osg::Vec4(0.f, 0.f, 0.f, 1.f));
        state->setAttribute(mat.get());

        mt->addChild(lozenge.get());
        root->addChild(mt.get());
    }

    osg::ref_ptr<osg::Geode> planeGeode = new osg::Geode;
    planeGeode->addDrawable(createPlane_4().get());
    {
        osg::StateSet *state = planeGeode->getOrCreateStateSet();

        osg::ref_ptr<osg::Material> mat = new osg::Material;
        mat->setDiffuse(osg::Material::FRONT,
                        osg::Vec4(.6f, .5f, .2f, 1.f));
        mat->setSpecular(osg::Material::FRONT,
                         osg::Vec4(.4f, .4f, .4f, 1.f));
        mat->setShininess(osg::Material::FRONT, 128.f);
        state->setAttribute(mat.get());

        root->addChild(planeGeode.get());
    }

    return root.get();
}
