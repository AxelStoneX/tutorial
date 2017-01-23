import QtQuick 2.0

Item
{
    width: 200
    height: 80
    TextEdit
    {
        anchors.left: parent.left
        anchors.right: parent.horizontalCenter
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        text: "TextEdit1\nTextEdit1\nTextEdit1"
        font.pixelSize: 20
        color: focus ? "red" : "black"
        focus: true
    }

    TextEdit
    {
        anchors.left: parent.horizontalCenter
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        text: "textEdit2\ntextEdit 2\nTextedit 2"
        font.pixelSize: 20
        color: focus ? "red" : "black"
    }
}

