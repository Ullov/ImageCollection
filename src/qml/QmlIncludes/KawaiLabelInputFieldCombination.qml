import QtQuick 2.11
import QtQuick.Controls 2.4

Rectangle {
    property alias label: label
    property alias textField: textField
    property alias decorativeRectangle: decorativeRectangle
    property int fontSize: 10
    property bool onlyRead: false
    id: root
    height: 37
    width: 300
    anchors.margins: 3
    Text {
        id: label
        anchors.left: parent.left
        anchors.top: parent.top
        font.pixelSize: fontSize
        font.family: fixedFont.name
        text: "Sample text"
        height: 18
    }

    ScrollView {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        TextArea {
            id: textField
            background: Rectangle {
                id: decorativeRectangle
                anchors.fill: parent
                border.color: textField.activeFocus ? "Black" : "#dfdfdf"
                color: textField.activeFocus ? "White" : "#edfeff"
                height: 20 // To silence warning
                width: 20 // To silence warning
            }
            font.pixelSize: fontSize
            font.family: fixedFont.name
            text: "Sample text"
            readOnly: onlyRead
            selectByMouse: true
        }
    }

    FontLoader {
        id: fixedFont
        source: "qrc:/resources/fonts/RobotoMono-Regular.ttf"
    }
}
