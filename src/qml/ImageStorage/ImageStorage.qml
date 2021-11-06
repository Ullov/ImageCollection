import QtQuick 2.0
import QtQuick.Controls 2.15

Rectangle {
    id: root
    anchors.fill: parent
    //color: "#34353b"
    TagWindow {
        id: tags
        windowSwitchHeight: windowSwitch.height
        windowSwitchWidth: windowSwitch.width
    }
    ComboBox {
        id: windowSwitch
        anchors.top: parent.top
        anchors.left: parent.left
        height: 40
        width: 100
        model: ListModel {
            ListElement { name: "Tags" } // id:0
            ListElement { name: "Images" } // id:1
        }
        onCurrentIndexChanged: {
            if (currentIndex === 0)
                tags.visible = true
            else if (currentIndex === 1)
                tags.visible = false
        }
    }
    Component.onCompleted: windowSwitch.currentIndex = 0
}
