import QtQuick 2.0
import QmlPixmapImage 1.0

Rectangle {
    property var extraParams
    id: root
    anchors.fill: parent
    color: "Blue"
    clip: true
    focus: true
    MouseArea {
        anchors.fill: parent
        drag.target: pixImage
        drag.axis: Drag.XAndYAxis
        PixmapImage {
            id: pixImage
            height: 3000
            width: 3000
            onWidthChanged: centerImage()
        }
        onWheel: {
            pixImage.setCanvasScale(wheel.angleDelta.y / 120)
        }
    }
    Keys.onPressed: {
        if (event.key === Qt.Key_Left)
            pixImage.previousPicture()
        else if (event.key === Qt.Key_Right)
            pixImage.nextPicture()
        else if (event.key === Qt.Key_C)
            centerImage()
    }
    Component.onCompleted: {
        pixImage.setViewerDimensions(root.height, root.width)
        pixImage.setImage(extraParams)
    }
    function centerImage()
    {
        pixImage.anchors.centerIn = clickArea
        pixImage.anchors.centerIn = undefined
    }
}
// E:/UgiU4MOllPU.jpg
