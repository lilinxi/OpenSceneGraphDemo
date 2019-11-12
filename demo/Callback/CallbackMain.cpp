// Callback Example, Using an update callback to modify the scene graph

#include <osgViewer/Viewer>
#include <osg/NodeCallback>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osg/Notify>

// Derive a class from NodeCallback to manipulate a
//   MatrixTransform object's matrix.
class RotateCB_7 : public osg::NodeCallback {
public:
    RotateCB_7() : _angle(0.) {}

    // 在每个更新遍历过程中，OSG 都会调用派生类中的 operator()方法，从而允许用户程序对 Node 进行修改。
    virtual void operator()(osg::Node *node, osg::NodeVisitor *nv) {
        // Normally, check to make sure we have an update
        //   visitor, not necessary in this simple example.
        osg::MatrixTransform *mtLeft = dynamic_cast<osg::MatrixTransform *>( node );
        osg::Matrix mR, mT;
        mT.makeTranslate(-6., 0., 0.);
        mR.makeRotate(_angle, osg::Vec3(0., 0., 1.));
        mtLeft->setMatrix(mR * mT);

        // Increment the angle for the next from.
        _angle += 0.01;

        // Continue traversing so that OSG can process
        //   any other nodes with callbacks.
        traverse(node, nv); // 继续遍历
    }

protected:
    double _angle;
};

// Create the scene graph. This is a Group root node with two
//   MatrixTransform children, which multiply parent a single
//   Geode loaded from the cow.osg model file.
osg::ref_ptr<osg::Node> createScene_7() {
    // Load the cow model.
    osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile(
            "/Users/limengfan/CLionProjects/OpenSceneGraphDemo/demo/CallbackMain/cow.osg");
    if (!cow.valid()) {
        osg::notify(osg::FATAL) << "Unable to load data file. Exiting." << std::endl;
        return NULL;
    }

    // osgViewer 支持的多线程模型允许用户程序主循环不必等到绘制遍历结束就可以继续运行。
    // 这就是说 Viewer::frame()方法在绘制遍历仍未结束的时候就可以返回。
    // 换句话说，上一帧的绘制遍历可以与下一帧的更新遍历产生交叠。

    // 数据的变度
    // 初始状态下，变度的值是 UNSPECIFIED。 用户程序可以将数据变度更改为 STATIC 或者 DYNAMIC。
    // OSG 将确保绘制遍历在所有的 DYNAMIC 节点和数据处理完成后才会返回。
    // 同样，由于绘制遍历在函数返回后仍然可以继续渲染场景图形，OSG 将确保此时只有 STATIC 数据可以继续进行图形渲染。

    // Data variance is STATIC because we won't modify it.
    cow->setDataVariance(osg::Object::STATIC);

    // Create a MatrixTransform to display the cow on the left.
    osg::ref_ptr<osg::MatrixTransform> mtLeft = new osg::MatrixTransform;
    mtLeft->setName("Left Cow\nDYNAMIC");
    // Set data variance to DYNAMIC to let OSG know that we
    //   will modify this node during the update traversal.
    mtLeft->setDataVariance(osg::Object::DYNAMIC);
    // Set the update callback.
    mtLeft->setUpdateCallback(new RotateCB_7);
    osg::Matrix m;
    m.makeTranslate(-6.f, 0.f, 0.f);
    mtLeft->setMatrix(m);
    mtLeft->addChild(cow.get());

    // Create a MatrixTransform to display the cow on the right.
    osg::ref_ptr<osg::MatrixTransform> mtRight = new osg::MatrixTransform;
    mtRight->setName("Right Cow\nSTATIC");
    // Data variance is STATIC because we won't modify it.
    mtRight->setDataVariance(osg::Object::STATIC);
    m.makeTranslate(6.f, 0.f, 0.f);
    mtRight->setMatrix(m);
    mtRight->addChild(cow.get());

    // Create the Group root node.
    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->setName("Root Node");
    // Data variance is STATIC because we won't modify it.
    root->setDataVariance(osg::Object::STATIC);
    root->addChild(mtLeft.get());
    root->addChild(mtRight.get());

    return root.get();
}

int main_7(int, char **) {
    // Create the viewer and set its scene data to our scene
    //   graph created above.
    osgViewer::Viewer viewer;
    viewer.setSceneData(createScene_7().get());
    if (!viewer.getSceneData()) {
        return 1;
    }

    // Set the clear color to something other than chalky blue.
    viewer.getCamera()->setClearColor(osg::Vec4(1., 1., 1., 1.));

    // Loop and render. OSG calls RotateCB::operator()
    //   during the update traversal.
    return viewer.run();
}
