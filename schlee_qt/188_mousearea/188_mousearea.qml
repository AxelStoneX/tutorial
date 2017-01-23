import QtQuick 2.0

Rectangle
{
    width: 360
    height: 200
    color: "lightgreen"
    Text
    {
        anchors.centerIn: parent
        text: "<h1><center>Click me!<br>
               (use left or right mouse button)</center></h1>"
    }

    MouseArea
    {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onPressed:
        {
            if (mouse.button == Qt.RightButton)
            {
                parent.color = "red"
            }
            else
            {
                parent.color = "blue"
            }
        }
        onReleased: parent.color = "lightgreen"
    }
}

