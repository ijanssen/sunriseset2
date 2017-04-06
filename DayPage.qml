import QtQuick 2.7
import QtQuick.Controls 2.0

Page {
    property date date
    property variant lengthOfDay: null
    property variant riseTime
    property variant setTime
    property variant riseCivilTime
    property variant setCivilTime
    property variant riseNauticalTime
    property variant setNauticalTime
    property variant riseAstronomicalTime
    property variant setAstronomicalTime
    property variant noonTime

    header: Button {
        text: '<<<'
        onClicked:
            stackView.pop();
    }

    function getTime(t)
    {
        //console.log(t);
        if (t === "")
            return "------";
        else
            return Qt.formatTime(t, 'hh:mm')
    }

    Label {
        id: header
        width: parent.width
        anchors.top: parent.top
        anchors.topMargin: 20
        font.pixelSize: 24
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        text: Qt.formatDate(date, 'dd-MM-yyyy')
    }

    Column {
        anchors {
            top: header.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            leftMargin: 16
            rightMargin: 16
        }
        Item {
            width: parent.width
            height: 30
        }
        Item {
            width: parent.width
            height: 30
            Label {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 16
                font.bold: true
                text: qsTr("Sunrise:")
            }
            Label {
                anchors.left: parent.left
                anchors.leftMargin: 200
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 16
                text: getTime(riseTime)
            }
        }

        Item {
            width: parent.width
            height: 30
            Label {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 16
                font.bold: true
                text: qsTr("Sunset:")
            }
            Label {
                anchors.left: parent.left
                anchors.leftMargin: 200
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 16
                text: getTime(setTime)
            }
        }
        Item {
            width: parent.width
            height: 20
        }

        Item {
            width: parent.width
            height: 30
            Label {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 16
                font.bold: true
                text: qsTr("Noon time:")
            }
            Label {
                anchors.left: parent.left
                anchors.leftMargin: 200
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 16
                text: Qt.formatTime(noonTime, 'hh:mm')
            }
        }

        Item {
            width: parent.width
            height: 30
            Label {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 16
                font.bold: true
                text: qsTr("Length on Day:")
            }
            Label {
                anchors.left: parent.left
                anchors.leftMargin: 200
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 16
                text: Qt.formatTime(lengthOfDay, 'hh:mm')
            }
        }

        Item {
            width: parent.width
            height: 30
        }


        HorizontalLine {
        }

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 24
            text: qsTr("Twilight")
        }

        RiseSetLine {
            text: qsTr("Civil:")
            riseTime: riseCivilTime
            setTime: setCivilTime
        }

        RiseSetLine {
            text: qsTr("Nautical:")
            riseTime: riseNauticalTime
            setTime: setNauticalTime
        }

        RiseSetLine {
            text: qsTr("Astronomical:")
            riseTime: riseAstronomicalTime
            setTime: setAstronomicalTime
        }
    }

}
