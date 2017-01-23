import QtQuick 2.0

Rectangle
{
    width: 300
    height: 100
    color: "lightgreen"

    Rectangle
    {
        x: 0
        y: 0
        height: 100
        color: "red"
        NumberAnimation on width
        {
            from: 300
            to: 0
            duration: 2000
            easing.type: Easing.InOutCubic
        }
    }
}

