import QtQuick 2.0
import QmlPixmapImage 1.0

Rectangle {
    property var extraParams
    id: root
    anchors.fill: parent
    color: "#787878"
    Flickable {
        anchors.fill: parent
        clip: true
        contentHeight: 10000
        contentWidth: 10000
        PixmapImage {
            id: pixImage
            height: 3000
            width: 3000
        }
    }
    Component.onCompleted: {
        pixImage.setImage(extraParams)
    }
}
// E:/UgiU4MOllPU.jpg
