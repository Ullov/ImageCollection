import QtQuick 2.0
import QtQuick.Controls 2.15

Rectangle {
    property alias label: name
    property alias comboBox: box
    property alias model: comboBoxModel
    property int fontSize: 11

    id: root
    height: 50
    width: 200
    Text {
        id: name
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        text: "Sample text"
        font.pixelSize: fontSize
    }
    ComboBox {
        id: box
        y: 0
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        width: 140
        height: 30
        model: comboBoxModel
    }
    ListModel {
        id: comboBoxModel
    }
}
