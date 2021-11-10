import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQml.Models 2.12
import "../QmlIncludes"

Rectangle {
    property int windowSwitchHeight: 40
    property int windowSwitchWidth: 100
    property string colorText: "White"
    property string colorBorder: "Black"
    property string colorFirst: "#34353b"
    property string colorSecond

    id: root
    anchors.fill: parent
    color: colorFirst
    Rectangle {
        id: searchRectangle
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 82
        color: colorFirst
        border.color: colorBorder
        border.width: 2
        KawaiLabelInputFieldCombination {
            id: keywordLine
            anchors.centerIn: parent
            label.text: "Keyword"
            textField.placeholderText: "Input some words..."
            textField.text: ""
            textField.color: colorText
            decorativeRectangle.color: colorFirst
            color: colorFirst
            label.color: colorText
            fontSize: 13
            height: 50
        }
        KawaiButton {
            id: searchButton
            anchors.left: keywordLine.right
            anchors.bottom: keywordLine.bottom
            anchors.leftMargin: 5
            height: 31
            radius: 2
            labelText: "Search"
        }
    }
    AddTagWindow {
        id: addTagRect
        anchors.top: searchRectangle.bottom
        anchors.left: parent.left
        colorText: colorText
        colorBorder: colorBorder
        colorFirst: colorFirst
    }
    ListView {
        id: tagsListView
        anchors.top: searchRectangle.bottom
        anchors.left: addTagRect.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 8
        model: model
        clip: true
        spacing: 3
    }
    DelegateModel {
        id: model
        model: ListModel {}
        delegate: delegateItem
    }
    Component {
        id: delegateItem
        DelegateItemForTags {
            width: tagsListView.width
        }
    }

    Component.onCompleted: {
        //var allTags = imageStorage.getAllTags()
        //refreshListView(allTags)
    }

    function refreshListView(allTags)
    {
        model.model.clear()
        for (var i = 0; allTags[i]; i++)
        {
            model.model.append(allTags[i])
        }
    }
}
