import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import com.avocado 1.0

ApplicationWindow {
    visible: true
    width: 480
    height: 1000
    title: qsTr("SunRiseSet")

    SunRiseSetModel {
        id: sunRiseSetModel
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        StackView {
            id: stackView
            //           anchors.fill: parent
            initialItem:ListPage {
            }
        }

        SettingsPage {
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        TabButton {
            text: qsTr("Calendar")
        }
        TabButton {
            text: qsTr("Settings")
        }
    }
}
