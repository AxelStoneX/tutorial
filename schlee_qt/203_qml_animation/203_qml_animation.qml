import QtQuick 2.0

Rectangle
{
    width: 400
    height: 400
    Image
    {
        id: img
        source: "icon1.png"
        smooth: true
        anchors.centerIn: parent
    }

    ParallelAnimation
    {
        NumberAnimation
        {
            target: img
            properties: "scale"
            from: 0.1;
            to: 3.0;
            duration: 2000
            easing.type: Easing.InOutCubic
        }
        NumberAnimation
        {
            target: img
            properties: "opacity"
            from: 1.0
            to: 0;
            duration: 2000
        }
        running: true
        loops: Animation.Infinite
    }
}

