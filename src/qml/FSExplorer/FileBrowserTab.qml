import QtQuick 2.13
import QtQuick.Controls 2.5
import QtQml.Models 2.2
import "FileListTools.js" as FileListTools

Rectangle {
    id: root
    property string uuid
    property string openedDir
    visible: true
    anchors.fill: parent

    signal signalOnSimpleLeftClick(int numb)

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
            onClicked: fsExplorerHandle.slotShowDrivesList(uuid)
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
                oneLineSelected.connect(root.simpleLeftClickFunction)
                root.signalOnSimpleLeftClick.connect(slotOnSimpleLeftClick)
            }
            Component.onDestruction: root.signalOnSimpleLeftClick.disconnect(slotOnSimpleLeftClick)
            onSelectedChanged: DelegateModel.inPersistedItems = 1
        }
    }
    DelegateModel {
        id: modelItem
        model: ListModel {}
        delegate: delegateItem
    }

    Connections {
        target: fsExplorerHandle
        function onDirInfo(dirInfo, expectedUuid) {
            if (expectedUuid === uuid)
            {
                FileListTools.formFilesList(dirInfo)
            }
        }
        function onDrivesList(drives, expectedUuid) {
            if (expectedUuid === uuid)
            {
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
    }

    Menu {
        id: driveMenu
    }

    Menu {
        id: contextMenu
        MenuItem {
            text: "Delete"
            onClicked: fsExplorerHandle.slotRemoveFile(FileListTools.returnSelectedFilesPaths(), uuid)
        }
        MenuItem {
            text: "Show file info"
            onClicked: openFileInfo()
        }
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: contextMenu.popup()
    }

    Component.onCompleted: {
        uuid = uuidsList.getAndMark()
        fsExplorerHandle.init(uuid)
    }
    Component.onDestruction: {
        uuidsList.unmark(uuid)
    }

    function simpleLeftClickFunction(numb)
    {
        signalOnSimpleLeftClick(numb)
    }

    function openFileInfo()
    {
        for (var i = 0; i < modelItem.count; i++)
        {
            if (modelItem.model.get(i).sel)
            {
                var info = modelItem.model.get(i)

                var obj = Qt.createComponent("qrc:/qml/FSExplorer/FileInfoWindow.qml")
                if (obj.status === Component.Error)
                    console.log(obj.errorString())
                obj.createObject(root, {info:info})

                break
            }
        }
    }
}
