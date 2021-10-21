import QtQuick 2.0
import QtQuick.Controls 2.5
import "../../js/Tools.js" as Tools

Rectangle {
    id: globalRoot
    anchors.fill: parent
    property var windowsObects: []
    property int currentWindowId: 0
    property int comboBoxCounter: 0
    property int windowIdForMenuAction: 0
    property int uuidNumber
    property string uuid
    enum MenuActionMode {
        MenuCall,
        Close,
        Duplicate
    }

    Rectangle {
        id: leftBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: Tools.cutNumberByPercents(parent.width, 15)
        ComboBox {
            id: selectTypeComboBox
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20
            model: comboBoxListModel
            textRole: "text"
            valueRole: "id"
            onCurrentIndexChanged: comboBoxSelectionChanged()
        }

        KawaiHorisontalLine {
            id: leftBarDivisionLine
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: selectTypeComboBox.bottom
            anchors.topMargin: 20
        }

        ListView {
            id: leftBarListView
            model: leftBarListViewModelItem
            delegate: leftBarListViewDelegateItem
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: leftBarDivisionLine.bottom
            anchors.bottom: parent.bottom
            anchors.margins: 5
            clip: true
            ScrollBar.vertical: ScrollBar {
                id: scrollBarItem
                policy: ScrollBar.AlwaysOn
            }
            Keys.onUpPressed: scrollBarItem.decrease()
            Keys.onDownPressed: scrollBarItem.increase()
            spacing: 3
        }
        ListModel {
            id: comboBoxListModel
        }
        ListModel {
            id: leftBarListViewModelItem
        }
        Component {
            id: leftBarListViewDelegateItem
            KawaiButton {
                width: leftBarListView.width - 15
                anchors.margins: 5
                labelText: buttonText
                borderRadiusC: 0
                pixelSize: 15
                heightC: 30
                mouseArea.acceptedButtons: Qt.LeftButton | Qt.RightButton
                mouseArea.onClicked: {
                    if (mouse.button === Qt.RightButton)
                    {
                        menuAction(KawaiSwitchableWindows.MenuActionMode.MenuCall, windowId)
                        contextMenu.popup()
                    }
                    else
                    {
                        callWindow(windowId)
                    }
                }
            }
        }

    }

    Rectangle {
        id: contentRectangle
        anchors.left: leftBar.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
    }

    Menu {
        id: contextMenu
        MenuItem {
            text: "Close"
            onClicked: menuAction(KawaiSwitchableWindows.MenuActionMode.Close, windowIdForMenuAction)
        }
        MenuItem {
            text: "Duplicate"
            onClicked: menuAction(KawaiSwitchableWindows.MenuActionMode.Duplicate, windowIdForMenuAction)
        }
    }

    Component.onCompleted: {
        uuid = uuidsList.getAndMark()
    }
    Component.onDestruction: {
        uuidsList.unmark(uuid)
    }

    signal menuAction(int mode, int windowId)
    Connections {
        target: globalRoot
        function onMenuAction(mode, windowId) {
            if (mode === KawaiSwitchableWindows.MenuActionMode.Close)
            {
                windowsObects[windowId][1].destroy()
                for (var i = 0; i < leftBarListViewModelItem.count; i++)
                {
                    var obj = leftBarListViewModelItem.get(i)
                    if (obj.windowId === windowId)
                    {
                        leftBarListViewModelItem.remove(i)
                        windowsObects[i][0] = -1
                    }
                }
            }
            else if (mode === KawaiSwitchableWindows.MenuActionMode.MenuCall)
            {
                windowIdForMenuAction = windowId
            }
            else if (mode === KawaiSwitchableWindows.MenuActionMode.Duplicate)
            {
                var obj
                for (var i = 0; i < leftBarListViewModelItem.count; i++)
                {
                    obj = leftBarListViewModelItem.get(i)
                    if (obj.windowId === windowId)
                    {
                        break
                    }
                }

                var params =
                        [
                            [
                                "Test",
                                [
                                    [obj["buttonText"], obj["qmlFilePath"], []]
                                ]
                            ]
                        ]
                addItemsAndSections(params)
            }
        }
    }


    function addItems(itemParams)
    {
        for (var i = 0 ; itemParams[i]; i++)
        {
            var winId = createAndAddObject(itemParams[i][1], itemParams[i][0], itemParams[i][2])
            leftBarListViewModelItem.append({buttonText: itemParams[i][0], windowId:winId, qmlFilePath:itemParams[i][1]})
        }
    }
    function createAndAddObject(path, name, extraParams)
    {
        var windowParams = {visible: false, extraParams: extraParams}
        var len = windowsObects.length
        var obj = Qt.createComponent(path)
        if (obj.status == Component.Error)
            console.log(obj.errorString())
        windowsObects.push([
            comboBoxCounter, // sectionId
            obj.createObject(contentRectangle, windowParams),
            len, // windowId
            name // buttonText
        ]);
        return len
    }
    function callWindow(windowId)
    {
        windowsObects[currentWindowId][1].visible = false;
        windowsObects[windowId][1].visible = true;
        currentWindowId = windowId
    }
    function addItemsAndSections(params)
    {
        for (var i = 0; params[i]; i++)
        {
            var isRepeat = false
            for (var n = 0; n < comboBoxListModel.count; n++)
            {
                if (comboBoxListModel.get(n).text === params[i][0])
                {
                    isRepeat = true
                    break
                }
            }
            addItems(params[i][1])
            if (!isRepeat)
            {
                comboBoxListModel.append({text:params[i][0], id:comboBoxCounter})
                comboBoxCounter++
            }
        }
    }
    function comboBoxSelectionChanged()
    {
        leftBarListViewModelItem.clear()
        var newId = comboBoxListModel.get(selectTypeComboBox.currentIndex).id
        for (var i = 0; windowsObects[i]; i++)
        {
            if (windowsObects[i][0] === newId)
                leftBarListViewModelItem.append({buttonText: windowsObects[i][3], windowId: windowsObects[i][2]})
        }
    }
}
