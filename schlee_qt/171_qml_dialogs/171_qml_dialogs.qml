import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.1

ApplicationWindow
{
    width: 200
    height: 100
    visible: true
    title: "Dialogs Demo"

    Button
    {
        width: parent.width
        height: parent.height
        text: "Click to start a color dialog"
        onClicked:
        {
            messageDialog.visible = false;
            colorDialog.visible = true
        }
    }

    ColorDialog
    {
        id: colorDialog
        visible: false
        modality: Qt.WindowModal
        title: "Select a color"
        color: "blue"
        onAccepted:
        {
            messageDialog.informativeText = "Selected color: " + color
            messageDialog.visible = true
        }
    }

    MessageDialog
    {
        id: messageDialog
        visible: false
        modality: Qt.NonModal
        title: "Message"
    }
}

