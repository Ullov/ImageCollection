import QtQuick 2.13
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "FileListTools.js" as FileListTools

Rectangle {
    id: root
    anchors.fill: parent
    TabView {
        id: tabs
        anchors.fill: parent
        Component.onCompleted: {
            addTab("Files", Qt.createComponent("FileBrowserTab.qml"))
        }

        function loadTab(siteFile, title){
            var cTab = currentIndex
            var t = addTab(title, Qt.createComponent(siteFile))
            currentIndex = cTab
        }

        style: TabViewStyle {
            frameOverlap: 1
            tabsMovable: true
            tab: Rectangle {
                color: styleData.selected ? "#484848" : "#dddddd"
                border.color: "steelblue"
                implicitWidth: Math.max(text.width + 4, 80)
                implicitHeight: 30
                Text {
                    id: text
                    anchors.centerIn: parent
                    text: styleData.title
                    color: styleData.selected ? "white" : "black"
                }
            }
            frame: Rectangle { color: "steelblue" }
        }
    }
}
