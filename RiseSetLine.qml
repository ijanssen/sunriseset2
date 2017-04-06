import QtQuick 2.7
import QtQuick.Controls 2.0

Item {
    property alias text: label.text
    property variant riseTime
    property variant setTime

    function getTime(t)
    {
        if (t === "")
            return "------";
        else
            return Qt.formatTime(t, 'hh:mm')
    }

    width: parent.width
    height: 30
    Label {
        id: label
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 16
        font.bold: true
    }


    Image {
        id: riseIcon
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 150
        source: "qrc:/images/icon_rise.png"
    }
    Label {
        id: riseLabel
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: riseIcon.right
        anchors.leftMargin: 10
        text: getTime(riseTime)
    }

    Image {
        id: setIcon
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 70
        source: "qrc:/images/icon_set.png"
    }
    Label {
        id: setLabel
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: setIcon.right
        anchors.leftMargin: 10
        text: getTime(setTime)
    }

}
