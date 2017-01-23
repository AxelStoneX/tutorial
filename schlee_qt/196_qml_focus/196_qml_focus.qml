import QtQuick 2.0

Rectangle
{
    width: 300
    height: 300
    color: focus ? "red" : "lightgreen"
    KeyNavigation.tab: childrect

    Rectangle
    {
        id: childrect
        width: 150
        height: 150
        color: focus ? "red" : "lightgreen"
        KeyNavigation.tab: parent
        focus: true

        Text
        {
            anchors.centerIn: parent
            text: "Press TAB"
        }

    }
}

