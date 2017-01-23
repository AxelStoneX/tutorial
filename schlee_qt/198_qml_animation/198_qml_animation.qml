import QtQuick 2.0

Rectangle
{
    color: "lightgreen"
    width: 300
    height: 300
    Image
    {
        id: img
        x: 0
        y: 0
        source: "icon1.png"
    }
    PropertyAnimation
    {
        target: img
        properties: "x,y"
        from: 0
        to: 300 - img.height
        duration: 1500
        running: true
        loops: Animation.Infinite
        easing.type: Easing.OutExpo
    }
}

