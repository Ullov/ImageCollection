import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import "qrc:/js/Tools.js" as Tools

Rectangle {
    id: root

    property bool selected: false
    signal oneLineSelected(int numb);

    height: 23

    MouseArea {
        id: mouseAreaItem
        anchors.fill: parent
        Rectangle {
            id: buttonRectangle
            anchors.fill: parent
            color: "White"

            CheckBox {
                id: selectCheckBox
                checked: selected
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                indicator.height: 20
                indicator.width: 20
                onCheckedChanged: sel = selected = checked
            }

            Image {
                id: icon
                source: iconPath
                anchors.left: selectCheckBox.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: 23
            }

            Text {
                id: itemNameLabel
                anchors.left: icon.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 2
                text: itemName
                width: 500

                color: "Black"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 12
                font.family: "Segoe UI"
            }

            Text {
                id: itemTypeLabel
                anchors.left: itemNameLabel.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                text: fileType + "\t" + Tools.bytesToHumanReadable(size, 1)

                color: "Black"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 12
                font.family: "Segoe UI"
            }
            states: [
                State {
                    name: "onHoveredRectangle"
                    when: mouseAreaItem.containsMouse == true & selected == false
                    PropertyChanges {
                        target: buttonRectangle
                        color: "#CDE2FC"
                    }
                },
                State {
                    name: "onSelected"
                    when: root.selected == true
                    PropertyChanges {
                        target: buttonRectangle
                        color: "#A0CBFF"
                    }
                }
            ]
        }
        onClicked: simpleLeftClick(mouse)
        onDoubleClicked: openFileOrFolder()
        onContainsMouseChanged: hov = containsMouse
        Component.onCompleted: hoverEnabled = true
    }

    function openFileOrFolder() {
        if (isDir)
            fsExplorerHandle.slotCd(itemName, uuid)
        else
            fsExplorerHandle.slotOpenInDefaultApp(filePath, fileType, uuid)
    }

    function simpleLeftClick(mouse) {
        if (mouse.modifiers & Qt.ControlModifier)
        {
            if (!sel)
                sel = selected = selectCheckBox.checked = true
        }
        else
            root.oneLineSelected(index);
    }

    function slotOnSimpleLeftClick(numb)
    {
        if (numb === index)
            sel = selected = selectCheckBox.checked = true
        else
            sel = selected = selectCheckBox.checked = false
    }
}
