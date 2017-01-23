import QtQuick 2.0

Rectangle
{
    color: "aqua"
    width: img.width
    height: img.height

    Image
    {
        id: img
        x: 0
        y: 0
        smooth: true
        source: "test.png"
        scale: 0.75
        rotation: -30.0
    }
}

