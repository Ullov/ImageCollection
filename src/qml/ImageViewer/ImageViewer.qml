import QtQuick 2.0
import QmlPixmapImage 1.0
import "../../js/Tools.js" as Tools

Rectangle {
    property var extraParams
    property var imageInfo
    id: root
    anchors.fill: parent
    color: "Blue"
    clip: true
    focus: true
    Rectangle {
        id: topRect
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 30
        border.color: "Black"
        Text {
            id: infoLabel
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 5
            font.pixelSize: 14
        }
    }

    MouseArea {
        id: clickArea
        anchors.top: topRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        drag.target: pixImage
        drag.axis: Drag.XAndYAxis
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        PixmapImage {
            id: pixImage
            height: 3000
            width: 3000
            onWidthChanged: centerImage()
        }
        onWheel: {
            pixImage.setCanvasScale(wheel.angleDelta.y / 120)
        }
        onClicked: {
            if (mouse.button === Qt.RightButton)
                openAddToStorageWindow()
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
        pixImage.setViewerDimensions(clickArea.height, clickArea.width)
        pixImage.setImage(extraParams)
    }
    onFocusChanged: console.log(focus)
    Connections {
        target: pixImage
        function onImageInfo(info)
        {
            infoLabel.text = info.width + "x" + info.height + " || " + Tools.bytesToHumanReadable(info.size) + " || " + info.path
            imageInfo = info
        }
    }

    function centerImage()
    {
        pixImage.anchors.centerIn = clickArea
        pixImage.anchors.centerIn = undefined
    }

    function openAddToStorageWindow()
    {
        var obj = Qt.createComponent("qrc:/qml/ImageViewer/AddToStorageWindow.qml")
        if (obj.status === Component.Error)
            console.log(obj.errorString())
        obj.createObject(root, {info:imageInfo})
    }
}
// E:/UgiU4MOllPU.jpg
