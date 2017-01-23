import QtQuick 2.0
import QtQuick.XmlListModel 2.0

Rectangle
{
    id: mainrect
    color: "gray"
    width: 200
    height: 360
    Component
    {
        id: delegate
        Item
        {
            width: mainrect.width
            height: 70
            Row
            {
                anchors.verticalCenter: parent.verticalCenter
                Image
                {
                    width: 64
                    height: 64
                    source: cover
                    smooth: true
                }
                Column
                {
                    Text {color: "white"; text: artist; font.pointSize: 12}
                    Text {color: "lightblue"; text: album; font.pointSize: 10}
                    Text {color: "yellow"; text: year; font.pointSize: 8}
                }
            }
        }
    }

    ListView
    {
        anchors.fill: parent
        focus: true
        header: Rectangle
        {
            width: parent.width
            height: 30
            gradient: Gradient
            {
                GradientStop {position: 0; color: "gray"}
                GradientStop {position: 0.7; color: "black"}
            }

            Text
            {
                anchors.centerIn: parent
                color: "gray"
                text: "CDs"
                font.bold: true
                font.pointSize: 20
            }
        }

        footer: Rectangle
        {
            width: parent.width
            height: 30
            gradient: Gradient
            {
                GradientStop {position: 0; color: "gray"}
                GradientStop {position: 0.7; color: "black"}
            }
        }

        highlight: Rectangle
        {
            width: parent.width
            color: "blue"
        }

        model: CDs{}
        delegate: delegate

     }
}





