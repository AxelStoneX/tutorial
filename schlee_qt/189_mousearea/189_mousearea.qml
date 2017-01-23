import QtQuick 2.0

Rectangle
{
    width: 200
    height: 200
    color: mousearea.containsMouse ? "red" : "lightgreen"

    Text
    {
        anchors.centerIn: parent
        text: "<H1>Hover Me!</H1>"
    }

    MouseArea
    {
        id: mousearea
        anchors.fill: parent
        hoverEnabled: true
    }
}
