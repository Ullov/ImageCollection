import QtQuick 2.0
import QmlPixmapImage 1.0

Rectangle {
    property var extraParams
    id: root
    anchors.fill: parent
    color: "Blue"
    clip: true
    Flickable {
        anchors.fill: parent
        contentHeight: pixImage.height + 500
        contentWidth: pixImage.width + 500
        contentX: contentHeight / 2
        contentY: contentWidth / 2
        MouseArea {
            anchors.fill: parent
            Rectangle {
                id: pixRectangle
                anchors.centerIn: parent
                height: pixImage.height / 2
                width: pixImage.width / 2
                color: "#787878"
                PixmapImage {
                    id: pixImage
                    height: 3000
                    width: 3000
                }
            }
            onWheel: {
                wheel.accepted = true
                pixImage.setCanvasScale(wheel.angleDelta.y / 120)
            }
        }
        onFlickStarted: cancelFlick()
    }
    Component.onCompleted: {
        pixImage.setViewerHeight(root.height)
        pixImage.setViewerWidth(root.width)
        pixImage.setImage(extraParams)
    }
}
// E:/UgiU4MOllPU.jpg
