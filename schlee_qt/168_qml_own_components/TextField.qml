import QtQuick 2.0

Rectangle
{
    property alias text: txt.text
    property string name: "textField"

    width: txt.width
    height: txt.height

    Text
    {
        id: txt
        x: 0
        y: 0
    }
}
