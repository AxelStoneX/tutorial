import QtQuick 2.0

Rectangle
{
    id: rect
    width: 360
    height: 360
    state: "State2"
    Text
    {
        id: txt
        anchors.centerIn: parent
    }
    states: [
        State {
            name: "State1"
            PropertyChanges
            {
                target: rect
                color: "lightgreen"
                width: 150
                height: 60
            }
            PropertyChanges
            {
                target: txt
                text: "State2: Click Me!"
            }
        },
        State
        {
            name: "State2"
            PropertyChanges
            {
                target: rect
                color: "yellow"
                width: 200
                height: 120
            }
            PropertyChanges
            {
                target: txt
                text: "State1: Click Me!"
            }
        }
    ]

    MouseArea
    {
        anchors.fill: parent
        onClicked:
            parent.state = (parent.state == "State1") ? "State2" : "State1"
    }
}

