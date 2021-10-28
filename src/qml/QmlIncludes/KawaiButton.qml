import QtQuick 2.0
import QtQuick.Controls 2.2

Rectangle {
    id:root
    property string labelText: "Sample text"
    property string onPressedColor: "Black"
    property string onHoveredColor: "Gray"
    property string standartColor: "#dadada"
    property string borderColorC: "Black"
    property string onPressedBorderColor: "Black"
    property string onHoveredBorderColor: "Black"
    property real onPressedOpacity: 1
    property real onHoveredOpacity: 1
    property string labelTextColor: "Black"
    //property string onPressedLabelTextColor: "Black"
    //property string onHoveredLabelTextColor: "Black"
    property int borderWidthC: 1
    property int borderRadiusC: 5
    property int heightC: 40
    property int widthC: 100
    property int labelTextWeight: Font.Normal
    property int pixelSize: 12
    property alias mouseArea: mouseAreaItem
    //id: buttonRectangle
    implicitWidth: widthC
    implicitHeight: heightC
    border.color: borderColorC
    border.width: borderWidthC
    radius: borderRadiusC
    color: standartColor
    Text {
        id: buttonLabel
        anchors.centerIn: parent
        text: labelText
        color: labelTextColor
        font.weight: labelTextWeight
        font.pixelSize: pixelSize
    }
    MouseArea {
        id: mouseAreaItem
        anchors.fill: parent
        hoverEnabled: true
    }

    states: [
        State {
            name: "onPressedButtonRectangle"
            when: mouseAreaItem.pressed == true
            PropertyChanges {
                target: root
                color: onPressedColor
                border.color: onPressedBorderColor
                opacity: onPressedOpacity
            }
        },
        State {
            name: "onHoveredButtonRectangle"
            when: mouseAreaItem.containsMouse == true
            PropertyChanges {
                target: root
                color: onHoveredColor
                border.color: onHoveredBorderColor
                opacity: onHoveredOpacity
            }
        }
    ]
}
