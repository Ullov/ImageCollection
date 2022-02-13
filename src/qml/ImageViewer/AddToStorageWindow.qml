import QtQuick 2.0
import QtQml.Models 2.12
import QtQuick.Controls 2.12
import "qrc:/qml/QmlIncludes" as KTools

Rectangle {
    property string colorText: "White"
    property string colorBorder: "Black"
    property string colorFirst: "#34353b"
    property var info

    id: root
    height: 650
    width: 400
    border.width: 2
    anchors.centerIn: parent

    FontLoader {
        id: fixedFont
        source: "qrc:/resources/fonts/RobotoMono-Regular.ttf"
    }
    KTools.Button {
        id: closeButton
        anchors.right: parent.right
        anchors.top: parent.top
        radius: 0
        text: "X"
        textSize: 20
        opacity: 0.3
        mouseArea.onClicked: root.destroy()
    }
    KTools.LabelInputFieldCombination {
        id: name
        anchors.top: closeButton.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 40
        label.text: "Name"
        textField.text: info.name
    }
    LabelComboBoxCombination {
        id: conversionFormat
        anchors.top: name.bottom
        anchors.left: name.left
        label.text: "Convert to"
        Component.onCompleted: {
            model.append({text: "Keep original"})
            model.append({text: "WEBP"})
            model.append({text: "PNG"})
            model.append({text: "JPG"})
            comboBox.currentIndex = 0
        }
    }
    KTools.LabelInputFieldCombination {
        id: conversionQuality
        anchors.top: name.bottom
        anchors.left: conversionFormat.right
        anchors.right: name.right
        width: parent.width - 40
        label.text: "Conversion quality"
        textField.text: "100"
    }
    KTools.LabelInputFieldCombination {
        id: addTagName
        anchors.top: conversionFormat.bottom
        anchors.right: addTagButton.left
        anchors.left: conversionFormat.left
        label.text: "Add tag"
        textField.text: ""
    }
    KTools.Button {
        id: addTagButton
        anchors.right: conversionQuality.right
        anchors.top: conversionFormat.bottom
        anchors.topMargin: 5
        height: 33
        radius: 0
        text: "Add"
        textSize: 10
        mouseArea.onClicked: addTagToListModel()
    }
    Rectangle {
        id: tagListViewRect
        anchors.top: addTagName.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 40
        height: 150
        border.color: "Black"
        clip: true
        ListView {
            id: tagsListView
            anchors.fill: parent
            anchors.margins: 8
            model: tagDelModel
            clip: true
            spacing: 3
        }
    }

    DelegateModel {
        id: tagDelModel
        model: ListModel {}
        delegate: delegateItem
    }
    KTools.LabelInputFieldCombination {
        id: addAttributeName
        anchors.top: tagListViewRect.bottom
        anchors.right: attributeTypeComboBox.left
        anchors.left: conversionFormat.left
        label.text: "Add attribute"
        textField.text: ""
    }
    ComboBox {
        id: attributeTypeComboBox
        anchors.right: addAttributeButton.left
        anchors.top: addAttributeName.top
        width: 100
        height: 30
        model: ListModel {
            ListElement { text:"INT" }
            ListElement { text:"TEXT" }
            ListElement { text:"REAL" }
            ListElement { text:"BOOL" }
            ListElement { text:"BLOB" }
        }
    }
    KTools.Button {
        id: addAttributeButton
        anchors.right: conversionQuality.right
        anchors.top: tagListViewRect.bottom
        anchors.topMargin: 5
        height: 33
        radius: 0
        text: "Add"
        textSize: 10
        mouseArea.onClicked: addAttirbuteToListModel()
    }
    Rectangle {
        id: attrListViewRect
        anchors.top: addAttributeName.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 40
        height: 150
        border.color: "Black"
        clip: true
        ListView {
            id: attrListView
            anchors.fill: parent
            anchors.margins: 8
            model: attrDelModel
            clip: true
            spacing: 3
        }
    }

    DelegateModel {
        id: attrDelModel
        model: ListModel {}
        delegate: attrDelegateItem
    }
    Component {
        id: delegateItem
        Rectangle {
            width: tagsListView.width
            height: 20
            border.color: "Black"
            Text {
                id: tagName
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 5
                font.pixelSize: 10
                text: tagNameForLabel
            }
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.RightButton | Qt.LeftButton
                onClicked:{
                    if (mouse.button === Qt.RightButton)
                        tagDelModel.model.remove(index)
                }
            }
        }
    }
    Component {
        id: attrDelegateItem
        Rectangle {
            width: tagsListView.width
            height: 20
            border.color: "Black"
            Text {
                id: tagName
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 5
                font.pixelSize: 10
                text: attrNameForLabel + " |Type: " + tagTypeForLabel
            }
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.RightButton | Qt.LeftButton
                onClicked:{
                    if (mouse.button === Qt.RightButton)
                        attrDelModel.model.remove(index)
                }
            }
        }
    }
    KTools.Button {
        id: storeImageButton
        anchors.top: attrListViewRect.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10
        radius: 0
        text: "Store image"
        //mouseArea.onClicked:
    }

    MouseArea {
        anchors.fill: parent
        drag.target: root
        drag.axis: Drag.XAndYAxis
        z: -1
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        onClicked: mouse.accepted = true
        onPressed: mouse.accepted = true
        onReleased: mouse.accepted = true
        onDoubleClicked: mouse.accepted = true
        onPositionChanged: mouse.accepted = true
        onPressAndHold: mouse.accepted = true
    }
    Component.onCompleted: {
        root.anchors.centerIn = undefined
    }

    function addTagToListModel(tagName)
    {
        if (addTagName.textField.text !== "")
        {
            tagDelModel.model.append({tagNameForLabel: addTagName.textField.text})
            addTagName.textField.text = ""
        }
    }
    function addAttirbuteToListModel(tagName)
    {
        if (addAttributeName.textField.text !== "")
        {
            attrDelModel.model.append({attrNameForLabel: addAttributeName.textField.text, tagTypeForLabel: attributeTypeComboBox.currentText})
            addAttributeName.textField.text = ""
        }
    }
}
