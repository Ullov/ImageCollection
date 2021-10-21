import QtQuick 2.15
import QtQuick.Window 2.15
import "QmlIncludes"

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Image Collection")
    color: "Black"
    visibility: "Maximized"
    Rectangle {
        id: tmp
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: tmp2.left
        KawaiSwitchableWindows {
            id: switchableWindow
        }
    }
    Rectangle {
        id: tmp2
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width / 2
        KawaiSwitchableWindows {
            id: switchableWindow2
        }
    }

    Component.onCompleted: {
        var conf =
                [
                    [
                        "Test",
                        [
                            ["FSExplorer", "qrc:/qml/FSExplorer/StartObject.qml", []],
                            ["FSExplorer2.0", "qrc:/qml/FSExplorer/StartObject.qml", []]
                        ]
                    ]
                ]
        switchableWindow.addItemsAndSections(conf)
        switchableWindow2.addItemsAndSections(conf)
    }
    Connections {
        target: fsExplorerHandle
        function onOpenImage(pix)
        {
            var conf =
                    [
                        [
                            "Pictures",
                            [
                                ["Picture", "qrc:/qml/ImageViewer/ImageViewer.qml", pix]
                            ]
                        ]
                    ]
            switchableWindow.addItemsAndSections(conf)
        }
    }
}
