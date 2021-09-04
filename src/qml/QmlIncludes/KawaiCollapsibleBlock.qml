import QtQuick 2.0

Rectangle {
    id: root
    property bool folded: true
    property int foldedHeight
    property int unfoldedHeight
    property int animationDuration: 300
    clip: true
    border.color: "Black"
    radius: 5
    height: foldedHeight
    KawaiButton {
        id: button
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 2
        width: 20
        height: 20
        labelText: "+"
        onClicked: {
            if (folded)
                folded = false
            else
                folded = true
        }
    }
    states: [
        State {
            name: "onFoldedFalse"
            when: folded == false
        },
        State {
            name: "onFoldedTrue"
            when: folded == true
        }
    ]
    transitions: [
        Transition {
            from: "onFoldedFalse"
            PropertyAnimation {
                target: root
                from: unfoldedHeight
                to: foldedHeight
                duration: animationDuration
                properties: "height"
            }
        },
        Transition {
            from: "onFoldedTrue"
            PropertyAnimation {
                target: root
                from: foldedHeight
                to: unfoldedHeight
                duration: animationDuration
                properties: "height"
            }
        }
    ]
}
