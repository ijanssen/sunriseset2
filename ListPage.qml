import QtQuick 2.7
import QtQuick.Controls 2.0
import com.avocado 1.0

Page {
    header: ComboBox {
        id: typeComboBox
        textRole: "key"
        model: ListModel {
            ListElement { key: qsTr("Official"); value: SunRiseSetModel.Offical }
            ListElement { key: qsTr("Civil"); value: SunRiseSetModel.Civil }
            ListElement { key: qsTr("Nautical"); value: SunRiseSetModel.Nautical }
            ListElement { key: qsTr("Astronomical"); value: SunRiseSetModel.Astronomical }
        }
        //onCurrentIndexChanged: sunRiseSetModel.setType(model.get(currentIndex).value)
    }
    ListView {
        id: listView
        anchors.fill: parent
        //width: parent.width
        //anchors.top: header.bottom
        //anchors.bottom: parent.bottom
        clip: true

        model: sunRiseSetModel

        delegate: ItemDelegate {
            height: 40
            width: parent.width
            highlighted: isToday

            function getTime(t)
            {
                //console.log(t);
                if (t === "")
                    return "------";
                else
                    return Qt.formatTime(t, 'hh:mm')
            }

            function getSunRise()
            {
                var t = typeComboBox.model.get(typeComboBox.currentIndex).value;

                if (t === SunRiseSetModel.Civil)
                    return riseCivilTime;
                if (t === SunRiseSetModel.Nautical)
                    return riseNauticalTime;
                if (t === SunRiseSetModel.Astronomical)
                    return riseAstronomicalTime;
                return riseTime;
            }
            function getSunSet()
            {
                var t = typeComboBox.model.get(typeComboBox.currentIndex).value;

                if (t === SunRiseSetModel.Civil)
                    return setCivilTime;
                if (t === SunRiseSetModel.Nautical)
                    return setNauticalTime;
                if (t === SunRiseSetModel.Astronomical)
                    return setAstronomicalTime;
                return setTime;
            }

            HorizontalLine {
                anchors.top: parent.top
            }
            Label {
                anchors {
                    left: parent.left
                    leftMargin: 16
                    verticalCenter: parent.verticalCenter
                }
                text: Qt.formatDate(date, 'dd-MM-yyyy')
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
                text: getTime(getSunRise())
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
                text: getTime(getSunSet())
            }


            onClicked: {
                stackView.push(Qt.resolvedUrl("DayPage.qml"), {date: date, lengthOfDay: lengthOfDay, noonTime: noonTime,
                                   riseTime: riseTime, setTime: setTime,
                                   riseCivilTime: riseCivilTime, setCivilTime: setCivilTime,
                                   riseNauticalTime: riseNauticalTime, setNauticalTime: setNauticalTime,
                                   riseAstronomicalTime: riseAstronomicalTime, setAstronomicalTime: setAstronomicalTime})
            }

        }



        onMovementEnded: {
            if(atYBeginning) {
                sunRiseSetModel.addMoreAtBeginning();
            }
            if(atYEnd) {
                sunRiseSetModel.addMoreAtEnd();
            }
        }
    }
}
