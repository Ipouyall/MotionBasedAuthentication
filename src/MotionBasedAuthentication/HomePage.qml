import QtQuick
import QtQuick.Controls
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import QtQuick.Dialogs
import QtQuick.Controls.Material
import MotionBasedAuthentication 1.0

Item {
    id: homePage

    width: parent.width
    height: parent.height

    MotionBasedAuthentication {
        id: motionBasedAuthentication

        onStatusChanged: function(stat) {
            dialog.show(stat)
        }

        onDataChanged: function(data) {
            textArea.text = data
        }
    }

    Label {
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: 60
        }
        width: parent.width * 0.6
        height: 50

        text: "Result:"
        horizontalAlignment: Qt.AlignCenter
    }

    Flickable {
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: 120
        }
        width: parent.width * 0.6
        height: parent.height * 0.3

        flickableDirection: Flickable.VerticalFlick

        TextArea.flickable: TextArea {
            id: textArea
            anchors.fill: parent

            text: "No data"

            readOnly: true
            wrapMode: TextArea.Wrap
        }
    }

    Row {
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: 460
        }
        spacing: 20

        Button {
            id: startRecordingBtn

            width: 140
            height: 50

            text: "Start Attempt"

            onClicked: {
                motionBasedAuthentication.startRecording()
            }
        }

        Button {
            id: endRecordingBtn

            width: 140
            height: 50

            text: "End Attempt"

            onClicked: {
                motionBasedAuthentication.endRecording()
            }
        }
    }

    Row {
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: 520
        }
        spacing: 20

        Button {
            id: startCapturingBtn

            width: 140
            height: 50

            text: "Start Pattern"

            onClicked: {
                motionBasedAuthentication.startPattern()
            }
        }

        Button {
            id: endCapturingBtn

            width: 140
            height: 50

            text: "End Pattern"

            onClicked: {
                motionBasedAuthentication.endPattern()
            }
        }
    }

    Column {
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: 570
        }
        spacing: 10

        Button { // TODO: remove this. the dialog should be apeared after end attempting
            id: authenticateBtn

            width: 300
            height: 50

            text: "Authenticate"

            onClicked: {
                motionBasedAuthentication.authenticate()
            }
        }

        Button {
            id: showResultBtn

            width: 300
            height: 50

            text: "Show Data"

            onClicked: {
                motionBasedAuthentication.showData()
            }
        }

        Button {
            id: showPatternBtn

            width: 300
            height: 50

            text: "Show Pattern"

            onClicked: {
                var component = Qt.createComponent("PatternPage.qml")
                var item = component.createObject()
                stackView.push(item)
            }
        }

        Button {
            id: reset

            width: 300
            height: 50

            text: "Reset"

            onClicked: {
                motionBasedAuthentication.showData()
            }
        }
    }

    Dialog {
        id: dialog
        anchors.centerIn: parent
        width: parent.width * 0.4
        height: parent.height * 0.3

        Label {
            id: dialogMessage

            anchors.fill: parent
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
        }

        function show(result) {
            if (result === true) {
                dialogMessage.text = "Successful"
                dialogMessage.color = "green"
            } else {
                dialogMessage.text = "Failure"
                dialogMessage.color = "red"
            }
            open()
        }
    }
}
