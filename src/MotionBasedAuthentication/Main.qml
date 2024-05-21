import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Controls.Material

import MotionBasedAuthentication 1.0

ApplicationWindow {
    visible: true
    title: qsTr("Motion-based Authentication")
    width: Screen.width
    height: Screen.height

    StackView {
        id: stackView
        initialItem: "HomePage.qml"
        anchors.fill: parent
    }

    /*Component {
        id: homePage
        HomePage {
        }
    }

    Component {
        id: patternPage
        PatternPage {
        }
    }*/
}
