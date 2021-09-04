import QtQuick 2.0
import QtQuick.Controls 2.2

KawaiButton {
    id:root
    property string siteFile
    property string tabTitle

    MouseArea {
        id: clickArea
        anchors.fill: parent
        onClicked: tabs.loadTab(root.siteFile, root.tabTitle)
    }
}
