import QtQuick 2.0

Flickable
{
    width: 150
    height: 150
    contentWidth: image.width;
    contentHeight: image.height;

    Image
    {
        id: image
        source: "test.png"
    }
}

