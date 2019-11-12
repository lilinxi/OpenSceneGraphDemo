// Viewer Example, A minimal OSG viewer

#include <osgViewer/Viewer>
#include <osgDB/ReadFile>

int main(int, char **) {
    // Create a Viewer.
    osgViewer::Viewer viewer;

    // Load a model and add it to the Viewer.
    viewer.setSceneData(osgDB::readNodeFile("/Users/limengfan/CLionProjects/OpenSceneGraphDemo/demo/Viewer/cow.osg"));
    if (!viewer.getSceneData()) {
        osg::notify(osg::FATAL) << "Unable to load data file. Exiting." << std::endl;
        return 1;
    }

    viewer.getCamera()->setProjectionMatrixAsPerspective(40., 1., 1., 100.);
    // 创建矩阵，指定到视点的距离。
    osg::Matrix trans;
    trans.makeTranslate(0., 0., -12.);
    // 旋转一定角度(弧度值)。
    double angle(0.);
    while (!viewer.done()) {
        // 创建旋转矩阵。
        osg::Matrix rot;
        rot.makeRotate(angle, osg::Vec3(1., 0., 0.));
        angle += 0.01;

        // 设置视口矩阵(旋转矩阵和平移矩阵连乘)。
        viewer.getCamera()->setViewMatrix(rot * trans);
        // 绘制下一帧
        viewer.frame();
    }
}
