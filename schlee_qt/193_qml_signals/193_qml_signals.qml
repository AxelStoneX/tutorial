import QtQuick 2.0

Item
{
    width: 150
    height: 100
    Button
    {
        anchors.centerIn: parent
        text: "Please, Click me!"
        onClickedChanged:
        {
            text = "Clicked!"
        }
    }
}

