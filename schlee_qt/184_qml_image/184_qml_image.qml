import QtQuick 2.0

Image
{
    id: img
    x: 0
    y: 0
    smooth: true
    source:"test.png"
    transform: [
        Scale {origin.x: width / 2
               origin.y: height / 2
               xScale: 0.75
               yScale: 0.75
        },
        Rotation {origin.x: width / 2
                  origin.y: height / 2
                  angle: -30.0
        }
    ]
}

