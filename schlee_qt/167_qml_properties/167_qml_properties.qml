import QtQuick 2.0

Item
{
    width: 200
    height: 200

    Item
    {
        id: myelement
        property string name: "My Element"
        property int ver: 1
        property real pi: 3.14159
        property bool condition: true
        property url link: "http://www.bhv.com/"
    }

    Text
    {
        x: 0
        y: 0
        text: myelement.name + "<br>"
              + myelement.ver + "<br>"
              + myelement.pi  + "<br>"
              + myelement.condition + "<br>"
              + myelement.link
    }
}
