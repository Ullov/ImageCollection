import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle {
    property alias klifcLabel: label
    property alias klifcTextField: textField
    property alias klifcDecorativeRectangle: decorativeRectangle
    height: 40
    width: 300
    border.color: "Black"
    Text {
        id: label
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: 5
        font.pixelSize: 14
    }
    TextArea {
        id: textField
        anchors.right: parent.right
        anchors.left: label.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: 5
        background: Rectangle {
            id: decorativeRectangle
            //border.color: textField.activeFocus ? "Grey" : "Transparent"
        }
        font.pixelSize: 15
    }
    KawaiHorisontalLine {
        anchors.top: textField.bottom
        anchors.left: textField.left
        anchors.right: textField.right
        border.color: textField.activeFocus ? "Grey" : "Transparent"
    }
}
