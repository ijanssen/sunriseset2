import QtQuick 2.7
import QtQuick.Controls 2.0
import QtPositioning 5.2

Page {
    header: Button {
        text: qsTr("Save")
        onClicked: {
            sunRiseSetModel.setCoordinates(latitude.text, longitude.text, timeZoneSlider.value);
        }
    }

    function tzToString(tz)
    {
        var r = Math.abs(tz);
        var h = Math.floor(r);
        var s = "";
        if (tz < 0)
            s = "-";
        else if (tz > 0)
            s = "+";

        if (r > h)
            s += h + ":30";
        else
            s += h + ":00";
        return s;
    }

    PositionSource {
        id: positionSource
        active: false
        // nmeaSource: "nmealog.txt"
        property bool flag : false
        onPositionChanged: {
            if (!flag)
                return
            console.log('MYMY', positionSource.name, positionSource.position.coordinate)
            latitude.text = positionSource.position.coordinate.latitude;
            longitude.text = positionSource.position.coordinate.longitude;
        }
        onActiveChanged: console.log('Active ', active)
    }

    /*BusyIndicator {
        anchors.centerIn: parent
        running: false
        visible: true
    }*/

    Column {
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        spacing: 16

        Label {
            text: qsTr("Latitude:")
        }
        TextField {
            id: latitude
            anchors{
                leftMargin: 50
                left: parent.left
                right: parent.right
            }
            text: sunRiseSetModel.latitude
            maximumLength: 80
        }
        Label {
            text: qsTr("Longitude:")
        }
        TextField {
            id: longitude
            anchors{
                leftMargin: 50
                left: parent.left
                right: parent.right
            }
            text: sunRiseSetModel.longitude
            maximumLength: 80
        }
        Button {
            id: updateButton
            enabled: positionSource.valid/* && !positionSource.active*/
            text: qsTr("Update via  GPS")
            onClicked: {
                positionSource.flag = true
                positionSource.update();
            }
            //onEnabledChanged:  console.log('Enabled', enabled)
        }

        HorizontalLine {
        }

        Label {
            text: qsTr("Time zone:")
        }
        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            width: 100
            height: 50
            color: "#2289E0"
            radius: 20
            Label {
                anchors.centerIn: parent
                color: "white"
                text: tzToString(timeZoneSlider.value)
            }
        }

        Slider {
            id: timeZoneSlider
            width: parent.width
            stepSize: 0.5
            from: -12
            to: 12
            value: sunRiseSetModel.timeZone
            onPositionChanged: console.log(position)
        }

        Button {
            text: qsTr("Update to local")
            onClicked: {
                timeZoneSlider.value = sunRiseSetModel.localTimeZone();
                //console.log(sunRiseSetModel.localTimeZone());
            }
        }
    }

}
