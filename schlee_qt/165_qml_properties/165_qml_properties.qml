import QtQuick 2.0

Item
{
    width: 360
    height: 360
    Rectangle
    {
        id: redirect
        color: "red"
        x: 0
        y: 0
        width: parent.width / 2
        height: parent.height / 2
    }

    Rectangle
    {
        color: "green"
        x: redirect.width
        y: redirect.height
        width: redirect.width
        height: redirect.height
    }
}

