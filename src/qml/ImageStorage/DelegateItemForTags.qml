import QtQuick 2.0

Rectangle {
    property string colorText: "White"
    property string colorBorder: "Black"
    property string colorFirst: "#34353b"

    id: root
    color: colorFirst
    height: 30
    border.width: 1
    radius: 2
    Text {
        id: tagName
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: 5
        font.pixelSize: 13
        text: id + " " + name
        color: colorText
        width: parent.width / 4
    }
    Text {
        id: tagParent
        anchors.left: tagName.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: 5
        font.pixelSize: 13
        text: parentTagId !== "" ? parentTagId * 1 : ""
        color: colorText
        width: parent.width / 4
    }
}
