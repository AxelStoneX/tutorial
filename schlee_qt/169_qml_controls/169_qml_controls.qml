import QtQuick 2.1
import QtQuick.Controls 1.1

ApplicationWindow
{
    width: 320
    height: 240
    visible: true
    title: "One Button"

    Button
    {
        text: "Quit"
        width: parent.width
        height: parent.height
        onClicked: Qt.quit();
    }
}

