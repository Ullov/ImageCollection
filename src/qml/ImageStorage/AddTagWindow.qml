import QtQuick 2.0
import "../QmlIncludes"

Rectangle {
    property string colorText: "White"
    property string colorBorder: "Black"
    property string colorFirst: "#34353b"

    id:root
    height: 250
    width: 300
    border.width: 2
    border.color: colorBorder
    color: colorFirst
    Text {
        id: title
        anchors.top: parent.top
        anchors.margins: 5
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 20
        text: "Add tag"
        color: colorText
    }
    KawaiLabelInputFieldCombination {
        id: name
        anchors.top: title.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 3
        label.text: "Name"
        textField.placeholderText: "You can use all except colon(:)..."
        textField.text: ""
        textField.color: colorText
        decorativeRectangle.color: colorFirst
        color: colorFirst
        label.color: colorText
        fontSize: 12
        height: 45
    }
    KawaiLabelInputFieldCombination {
        id: parentTag
        anchors.top: name.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 3
        label.text: "Parent tag"
        textField.placeholderText: "Separate tags by colon(:)..."
        textField.text: ""
        textField.color: colorText
        decorativeRectangle.color: colorFirst
        color: colorFirst
        label.color: colorText
        fontSize: 12
        height: 45
    }
    KawaiLabelInputFieldCombination {
        id: summary
        anchors.top: parentTag.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 3
        label.text: "Summary"
        textField.placeholderText: "What interesting about this tag?.."
        textField.text: ""
        textField.color: colorText
        decorativeRectangle.color: colorFirst
        color: colorFirst
        label.color: colorText
        fontSize: 12
        height: 45
    }
    KawaiButton {
        id: addButton
        anchors.top: summary.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 3
        height: 31
        radius: 2
        labelText: "Add"
        mouseArea.onClicked: sendInfo()
    }
    Text {
        id: resultLabel
        anchors.top: addButton.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 5
        font.pixelSize: 15
        text: ""
        color: colorText
    }

    function sendInfo()
    {
        var info = {
            name: name.textField.text,
            parentTag: parentTag.textField.text,
            summary: summary.textField.text
        }
        resultLabel.text = imageStorage.addTag(info);
    }
}
