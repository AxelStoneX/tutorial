import QtQuick 2.2
import QtQuick.Controls 1.1

ApplicationWindow
{
    visible: true
    width: 200
    height: 75
    title: "Controls"

    menuBar: MenuBar
    {
        Menu
        {
            title: "File"
            MenuItem
            {
                text: "Quit"
                onTriggered: Qt.quit();
            }
        }
    }

    ProgressBar
    {
        x: 0
        y: 0
        width: parent.width
        height: parent.height / 2
        value: slider.value
    }

    Slider
    {
        id: slider
        x: 0
        y: parent.height / 2
        width: parent.width
        height: parent.height / 2
        value: 0.75
        tickmarksEnabled: true
        stepSize: 0.1
    }

}

