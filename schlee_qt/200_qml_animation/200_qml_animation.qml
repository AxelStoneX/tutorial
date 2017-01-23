import QtQuick 2.0

Rectangle
{
    width: 200
    height: 200
    ColorAnimation on color
    {
        from: Qt.rgba(1, 0.5, 0, 1)
        to: Qt.rgba(0.5, 0, 1, 1)
        duration: 1500
        running: true
        loops: Animation.Infinite
    }
}

