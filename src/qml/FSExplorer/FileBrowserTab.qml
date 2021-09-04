import QtQuick 2.13
import QtQuick.Controls 2.5
import "FileListTools.js" as FileListTools

Rectangle {
    id: root
    visible: true
    anchors.fill: parent

    signal signalOnSimpleLeftClick(int numb)

    Component.onCompleted: fsExplorerHandle.init()
    Rectangle {
        id: topBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 50
        color: "#15DDCA"
        Button {
            id: driveButton
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 10
            width: 30
            height: 30
            text: "..."
            onClicked: fsExplorerHandle.slotShowDrivesList()
        }
        TextField {
            id: addressTextField
            anchors.left: driveButton.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 10
            placeholderText: "Input some address..."
            height: 30
        }
    }

    ListView {
        id: listViewItem
        model: modelItem
        delegate: delegateItem
        anchors.top: topBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 5
        clip: true
        ScrollBar.vertical: ScrollBar {
            id: scrollBarItem
            policy: ScrollBar.AlwaysOn
        }
        Keys.onUpPressed: scrollBarItem.decrease()
        Keys.onDownPressed: scrollBarItem.increase()
    }

    Component {
        id: delegateItem
        DelegateItemForFolderList {
            width: listViewItem.width
            Component.onCompleted: {
                oneLineSelected.connect(root.simpleLestClickFunction)
                root.signalOnSimpleLeftClick.connect(slotOnSimpleLeftClick)
            }
            Component.onDestruction: root.signalOnSimpleLeftClick.disconnect(slotOnSimpleLeftClick)
        }
    }
    ListModel {
        id: modelItem
    }
    Connections {
        target: fsExplorerHandle
        function onDirInfo(dirInfo) {
            addressTextField.text = dirInfo["currentDir"]
            FileListTools.formFilesList(dirInfo)
        }
        function onDrivesList(drives) {
            var drivesNames = []
            var actions = []
            for (var i = 0; drives["dir"][i]; i++)
            {
                drivesNames.push(drives["dir"][i]["filePath"])
                actions.push(function(){fsh.slotCd(drivesNames[i])})
            }
            FileListTools.createDrivesMenu(drivesNames, actions, driveButton.x, driveButton.y + driveButton.height)
        }
    }

    Menu {
        id: driveMenu
    }

    Menu {
        id: contextMenu
        MenuItem {
            text: "Delete"
            onClicked: fsh.slotRemoveFile(Tools.returnSelectedFilesPaths())
        }
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: contextMenu.popup()
    }
    function simpleLestClickFunction(numb)
    {
        signalOnSimpleLeftClick(numb)
    }
}
