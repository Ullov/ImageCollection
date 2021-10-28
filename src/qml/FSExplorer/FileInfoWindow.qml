import QtQuick 2.15
import QtQuick.Shapes 1.15
import "../QmlIncludes"
import "../../js/Tools.js" as Tools

Rectangle {
    property var info
    property int borderWidth: 30
    property int outerLine: 2
    property string innerColor: "#b6fdff"
    property string outerColor: "#00f7ff"

    FontLoader {
        id: fixedFont
        source: "qrc:/resources/fonts/RobotoMono-Regular.ttf"
    }

    id: root
    anchors.centerIn: parent
    height: 500
    width: 400
    border.color: "Black"
    border.width: outerLine
    focus: true
    color: innerColor
    Rectangle {
        id: top
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: outerLine
        anchors.leftMargin: outerLine
        anchors.rightMargin: outerLine
        height: borderWidth
        gradient: LinearGradient {
            GradientStop { position: 0.0; color: outerColor}
            GradientStop { position: 1.0; color: innerColor}
        }
        KawaiButton {
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: parent.height
            borderRadiusC: 0
            labelText: "X"
            pixelSize: 20
            standartColor: innerColor
            opacity: 0.3
            mouseArea.onPressed: root.destroy()
        }
    }
    Rectangle {
        id: bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: outerLine
        anchors.rightMargin: outerLine
        anchors.bottomMargin: outerLine
        height: borderWidth
    }

    Rectangle {
        id: infoRectangle
        anchors.top: top.bottom
        anchors.bottom: bottom.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: outerLine
        color: innerColor
        Rectangle{
            id: icon
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 10
            height: 80
            width: 80
            border.color: "Black"
            Image {
                anchors.fill: parent
                source: info.iconPath
            }
        }

        KawaiLabelInputFieldCombination {
            id: infoFileName
            anchors.left: icon.right
            anchors.right: parent.right
            anchors.top: parent.top
            label.text: "File name"
            textField.text: info.fileName
            color: innerColor
        }
        KawaiLabelInputFieldCombination {
            id: infoPath
            anchors.left: icon.right
            anchors.right: parent.right
            anchors.top: infoFileName.bottom
            label.text: "Path"
            textField.text: info.path
            color: innerColor
            onlyRead: true
        }
        KawaiLabelInputFieldCombination {
            id: infoFilePath
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: icon.bottom
            label.text: "File Path"
            textField.text: info.filePath
            color: innerColor
            onlyRead: true
        }
        KawaiLabelInputFieldCombination {
            id: infoSize
            anchors.left: parent.left
            anchors.top: infoFilePath.bottom
            label.text: "Size"
            textField.text: Tools.bytesToHumanReadable(info.size) + "(" + info.size + " Bytes)"
            color: innerColor
            onlyRead: true
            width: 200
        }
        KawaiLabelInputFieldCombination {
            id: infoCreationTime
            anchors.top: infoSize.bottom
            anchors.left: parent.left
            label.text: "Creation Time"
            textField.text: info.whenCreated
            color: innerColor
            onlyRead: true
            width: 200
        }
        KawaiLabelInputFieldCombination {
            id: infoModificationTime
            anchors.top: infoCreationTime.bottom
            anchors.left: parent.left
            label.text: "Modification Time"
            textField.text: info.whenModified
            color: innerColor
            onlyRead: true
            width: 200
        }
        KawaiLabelInputFieldCombination {
            id: infoMetadataModificationTime
            anchors.top: infoModificationTime.bottom
            anchors.left: parent.left
            label.text: "Metadata Modification Time"
            textField.text: info.whenMetadataChanged
            color: innerColor
            onlyRead: true
            width: 200
        }
        KawaiLabelInputFieldCombination {
            id: infoReadedTime
            anchors.top: infoMetadataModificationTime.bottom
            anchors.left: parent.left
            label.text: "Last Reading Time"
            textField.text: info.whenReaded
            color: innerColor
            onlyRead: true
            width: 200
        }
        KawaiVerticalLine {
            id: verticalLine
            anchors.top: infoFilePath.bottom
            anchors.bottom: infoReadedTime.bottom
            anchors.left: infoReadedTime.right
            anchors.margins: 3
            anchors.bottomMargin: 0
        }
        KawaiLabelInputFieldCombination {
            id: infoPermissions
            anchors.top: infoFilePath.bottom
            anchors.left: verticalLine.left
            anchors.right: parent.right
            label.text: "Permissions"
            textField.text: permissionsToString()
            color: innerColor
            onlyRead: true
        }
        KawaiLabelInputFieldCombination {
            id: infoOwner
            anchors.top: infoPermissions.bottom
            anchors.left: verticalLine.left
            anchors.right: parent.right
            label.text: "Owner"
            textField.text: info.owner + "(" + info.ownerId + ")"
            color: innerColor
            onlyRead: true
        }
        KawaiLabelInputFieldCombination {
            id: infoGroup
            anchors.top: infoOwner.bottom
            anchors.left: verticalLine.left
            anchors.right: parent.right
            label.text: "Group"
            textField.text: info.group + "(" + info.groupId + ")"
            color: innerColor
            onlyRead: true
        }
    }

    MouseArea {
        id:mouse
        anchors.fill: parent
        drag.target: root
        drag.axis: Drag.XAndYAxis
        z: -1 // it should be in the very bottom
    }

    Component.onCompleted: {
        root.anchors.centerIn = undefined
    }

    function permissionsToString()
    {
        var str = ""
        if (info.isReadable)
            str += "R"
        if (info.isWritable)
            str += "W"
        if (info.isHidden)
            str += "H"
        if (info.isJunction)
            str += "J"
        if (info.isShortcut)
            str += "S"
        if (info.isSymLink)
            str += "L"
        return str
    }
}
