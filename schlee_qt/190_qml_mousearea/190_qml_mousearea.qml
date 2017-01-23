import QtQuick 2.0

Rectangle
{
    width: 200
    height: 200
    color: "lightgreen"
    Text
    {
        text: "<H1>Hover Me!</H1>"
        anchors.centerIn: parent
    }

    MouseArea
    {
        id: mousearea
        anchors.fill: parent
        hoverEnabled: true
        onEntered: parent.color = "red"
        onExited: parent.color = "lightgreen"
    }
}

