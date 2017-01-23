import QtQuick 2.0
import QtQuick.Layouts 1.1

Item
{
    width: 320
    height: 240

    RowLayout
    {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Rectangle
        {
            Layout.fillHeight: true
            Layout.minimumWidth: 64;
            Layout.minimumHeight: 64;
            color: "red"
        }

        Rectangle
        {
            Layout.fillWidth: true
            Layout.minimumHeight: 64
            Layout.minimumWidth: 64
            color: "blue"
        }
        Rectangle
        {
            Layout.fillHeight: true
            Layout.minimumWidth: 64;
            Layout.minimumHeight: 64;
            color: "green"
        }
    }
}

