// FindNode Example, Simple NodeVisitor-derived class for finding a named Node

// 正如 Callback 代码中演示的那样，由于关联更新回调到一个已知节点的方式十分容易，因此动态修改节点的方法也比较简单。
// 但是当用户程序要修改场景图形内部的某个节点，而且整个场景图形又是从外部模型文件读入的时候，
// 或者用户要交互地将进行选择的时候，问题就显得比较复杂了。
// 下面给出一些 OSG 在运行时进行节点识别的方法。

#include <osgViewer/Viewer>
#include <osg/Camera>
#include <osg/StateSet>
#include <osg/ShadeModel>
#include <osgDB/ReadFile>
#include <string>

// Derive a class from NodeVisitor to find a node with a
//   specific name.
class FindNamedNode_8 : public osg::NodeVisitor {
public:
    FindNamedNode_8(const std::string &name) :
            osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
            _name(name) {}

    // This method gets called for every node in the scene
    //   graph. Check each node to see if its name matches
    //   out target. If so, save the node's address.
    virtual void apply(osg::Node &node) {
        if (node.getName() == _name) {
            _node = &node;
        }

        // Keep traversing the rest of the scene graph.
        traverse(node);
    }

    osg::Node *getNode() { return _node.get(); }

protected:
    std::string _name;
    osg::ref_ptr<osg::Node> _node;
};

int main(int, char **) {
    osg::ref_ptr<osg::Node> sg = osgDB::readNodeFile(
            "/Users/limengfan/CLionProjects/OpenSceneGraphDemo/demo/FindNode/State.osg");
    if (!sg.valid()) {
        osg::notify(osg::FATAL) << "Unable to load data file. Exiting." << std::endl;
        return 1;
    }

    // 它将查找 StateSet 已经被设定为单一着色的 MatrixTransform 节点，而后改变其渲染状态为平滑着色。
    // Find the node who's name is "Flat".
    FindNamedNode_8 fnn("Flat");
    sg->accept(fnn);
    if (fnn.getNode() != NULL) {
        // We found the node. MGet the ShadeModel attribute
        //   from its StateSet and set it to SMOOTH shading.
        osg::StateSet *ss = fnn.getNode()->getOrCreateStateSet();
        osg::ShadeModel *sm = dynamic_cast<osg::ShadeModel *>(ss->getAttribute(osg::StateAttribute::SHADEMODEL));
        sm->setMode(osg::ShadeModel::SMOOTH);
    }

    // Create the viewer and set its scene data to our scene
    //   graph created above.
    osgViewer::Viewer viewer;
    viewer.setSceneData(sg.get());

    // Loop and render. OSG calls RotateCB::operator()
    //   during the update traversal.
    viewer.run();
}
