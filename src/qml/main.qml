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
    KawaiSwitchableWindows {
        id: switchableWindow
    }

    Component.onCompleted: {
        var conf =
                [
                    [
                        "Test",
                        [
                            ["FSExplorer", "qrc:/qml/FSExplorer/StartObject.qml", []],
                        ]
                    ]
                ]
        switchableWindow.addItemsAndSections(conf)
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
