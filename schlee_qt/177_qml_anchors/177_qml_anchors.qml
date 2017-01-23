import QtQuick 2.0

Item
{
    width: 360
    height: 360

    Rectangle
    {
        id: redrect
        color: "red"
        width: parent.width / 1.5
        height: parent.height / 1.5
        anchors.top: parent.top
        anchors.left: parent.left
    }

    Rectangle
    {
        opacity: 0.5
        color: "green"
        anchors.top: redrect.verticalCenter
        anchors.bottom: parent.bottom
        anchors.left: redrect.horizontalCenter
        anchors.right: parent.right
    }
}

