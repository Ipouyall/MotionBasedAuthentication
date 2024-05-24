import QtQuick
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Controls.Material

import MotionBasedAuthentication 1.0

ApplicationWindow {
    visible: true
    title: qsTr("Motion-based Authentication")
    // width: Screen.width
    // height: Screen.height
    width: 480
    height: 800
    color: "white"

    StackView {
        id: stackView
        initialItem: "HomePage.qml"
        anchors.fill: parent
    }
}
