// Picking Example, Using the osgUtil Intersection classes and osgGA NodeKit
// 点击物体旋转，点击空白取消
// osgGA 接收鼠标输入，osgUtil 计算相交

#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgUtil/PolytopeIntersector>
#include <osg/NodeCallback>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <iostream>
#include <osg/Notify>


osg::ref_ptr<osg::Node> _selectedNode_9;

// Derive a class from NodeCallback to manipulate a
//   MatrixTransform object's matrix.
class RotateCB_9 : public osg::NodeCallback {
public:
    RotateCB_9() : _angle(0.) {}

    virtual void operator()(osg::Node *node, osg::NodeVisitor *nv) {
        // Normally, check to make sure we have an update
        //   visitor, not necessary in this simple example.
        osg::MatrixTransform *mt = dynamic_cast<osg::MatrixTransform *>( node );
        osg::Matrix m;
        m.makeRotate(_angle, osg::Vec3(0., 0., 1.));
        mt->setMatrix(m);

        // Increment the angle for the next from.
        _angle += 0.01;

        // Continue traversing so that OSG can process
        //   any other nodes with callbacks.
        traverse(node, nv);
    }

protected:
    double _angle;
};

// Create the scene graph. This is a Group root node with two
//   MatrixTransform children, which multiply parent a single
//   Geode loaded from the cow.osg model file.
osg::ref_ptr<osg::Node> createScene_9() {
    // Load the cow model.
    osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile(
            "/Users/limengfan/CLionProjects/OpenSceneGraphDemo/demo/Picking/cow.osg");
    if (!cow.valid()) {
        osg::notify(osg::FATAL) << "Unable to load data file. Exiting." << std::endl;
        return NULL;
    }
    // Data variance is STATIC because we won't modify it.
    cow->setDataVariance(osg::Object::STATIC);

    // Create a MatrixTransform to display the cow on the left.
    osg::ref_ptr<osg::MatrixTransform> mtLeft = new osg::MatrixTransform;
    mtLeft->setName("Left Cow");
    mtLeft->setDataVariance(osg::Object::STATIC);
    osg::Matrix m;
    m.makeTranslate(-6.f, 0.f, 0.f);
    mtLeft->setMatrix(m);

    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
    mt->setName("Left Rotation");
    mt->setDataVariance(osg::Object::STATIC);
    m.makeIdentity();
    mt->setMatrix(m);

    mtLeft->addChild(mt.get());
    mt->addChild(cow.get());

    // Create a MatrixTransform to display the cow on the right.
    osg::ref_ptr<osg::MatrixTransform> mtRight =
            new osg::MatrixTransform;
    mtRight->setName("Right Cow");
    mtRight->setDataVariance(osg::Object::STATIC);
    m.makeTranslate(6.f, 0.f, 0.f);
    mtRight->setMatrix(m);

    mt = new osg::MatrixTransform;
    mt->setName("Right Rotation");
    mt->setDataVariance(osg::Object::STATIC);
    m.makeIdentity();
    mt->setMatrix(m);

    mtRight->addChild(mt.get());
    mt->addChild(cow.get());

    // Create the Group root node.
    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->setName("Root Node");
    // Data variance is STATIC because we won't modify it.
    root->setDataVariance(osg::Object::STATIC);
    root->addChild(mtLeft.get());
    root->addChild(mtRight.get());

    return root.get();
}

// PickHandler -- A GUIEventHandler that implements picking.
class PickHandler_9 : public osgGA::GUIEventHandler {
public:

    PickHandler_9() : _mX(0.), _mY(0.) {}

    bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {
        osgViewer::Viewer *viewer = dynamic_cast<osgViewer::Viewer *>( &aa );
        if (!viewer) {
            return false;
        }

        switch (ea.getEventType()) {
            case osgGA::GUIEventAdapter::PUSH:
            case osgGA::GUIEventAdapter::MOVE: {
                // Record mouse location for the button press
                //   and move events.
                _mX = ea.getX();
                _mY = ea.getY();
                return false;
            }
            case osgGA::GUIEventAdapter::RELEASE: {
                // If the mouse hasn't moved since the last
                //   button press or move event, perform a
                //   pick. (Otherwise, the trackball
                //   manipulator will handle it.)
                if (_mX == ea.getX() && _mY == ea.getY()) { // 如果鼠标位置没有移动，视为一次点击
                    if (pick(ea.getXnormalized(), ea.getYnormalized(), viewer)) {
                        return true;
                    }
                }
                return false;
            }

            default:
                return false;
        }
    }

protected:
    // Store mouse xy location for button press & move events.
    float _mX, _mY;

    // Perform a pick operation.
    bool pick(const double x, const double y, osgViewer::Viewer *viewer) {
        if (!viewer->getSceneData()) {
            // Nothing to pick.
            return false;
        }

        double w(.05), h(.05);
        osgUtil::PolytopeIntersector *picker =
                new osgUtil::PolytopeIntersector(
                        osgUtil::Intersector::PROJECTION,
                        x - w, y - h, x + w, y + h);

        osgUtil::IntersectionVisitor iv(picker); // as a NodeVisitor
        viewer->getCamera()->accept(iv);

        if (picker->containsIntersections()) {
            const osg::NodePath &nodePath = picker->getFirstIntersection().nodePath;
            unsigned int idx = nodePath.size();
            while (idx--) {
                // 查找交集节点路径中的最后一个 MatrixTransform;
                // 它就是将要与回调相关联的选择结果。
                // Find the LAST MatrixTransform in the node
                //   path; this will be the MatrixTransform
                //   to attach our callback to.
                osg::MatrixTransform *mt = dynamic_cast<osg::MatrixTransform *>(nodePath[idx]);
                if (mt == NULL){
                    continue;
                }

                // If we get here, we just found a
                //   MatrixTransform in the nodePath.

                if (_selectedNode_9.valid()){
                    // Clear the previous selected node's
                    //   callback to make it stop spinning.
                    _selectedNode_9->setUpdateCallback(NULL);
                }

                _selectedNode_9 = mt;
                _selectedNode_9->setUpdateCallback(new RotateCB_9);
                break;
            }
            if (!_selectedNode_9.valid()){
                osg::notify() << "Pick failed." << std::endl;
            }
        } else if (_selectedNode_9.valid()) {
            _selectedNode_9->setUpdateCallback(NULL);
            _selectedNode_9 = NULL;
        }
        return _selectedNode_9.valid();
    }
};

int main_9(int argc, char **argv) {
    // create the view of the scene.
    osgViewer::Viewer viewer;
    viewer.setSceneData(createScene_9().get());

    viewer.getCamera()->setClearColor(osg::Vec4(1., 1., 1., 1.));

    // add the pick handler
    viewer.addEventHandler(new PickHandler_9);

    return viewer.run();
}

