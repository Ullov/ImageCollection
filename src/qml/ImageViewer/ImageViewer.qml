import QtQuick 2.0
import QmlPixmapImage 1.0

Rectangle {
    property var extraParams
    id: root
    anchors.fill: parent
    color: "Blue"
    clip: true
    MouseArea {
        anchors.fill: parent
        drag.target: pixImage
        drag.axis: Drag.XAndYAxis
        PixmapImage {
            id: pixImage
            height: 3000
            width: 3000
        }
        onWheel: {
            pixImage.setCanvasScale(wheel.angleDelta.y / 120)
        }
    }
    Component.onCompleted: {
        pixImage.setViewerDimensions(root.height, root.width)
        pixImage.setImage(extraParams)
    }
}
// E:/UgiU4MOllPU.jpg
