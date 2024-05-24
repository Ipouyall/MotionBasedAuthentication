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
            statTextShort.text = stat ? "Successfully Authenticated!" : "Authentication Failed!"
            statTextShort.color = stat ? "green" : "red"
            dialog.show(stat)
        }

        onDataChanged: function(data) {
            textStatArea.text = data
        }
    }

    Label {
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: 10
        }
        id: statTextShort
        width: parent.width * 0.6
        height: 50

        text: "Authentication result isn't available"
        horizontalAlignment: Qt.AlignCenter
    }

    Rectangle {
        id: flickableContainer
        visible: false
        width: 360
        height: 270
        anchors.centerIn: parent
        anchors.topMargin: 300

        Flickable {
            width: parent.width
            height: parent.height
            anchors.verticalCenter: parent.top

            flickableDirection: Flickable.VerticalFlick

            TextArea.flickable: TextArea {
                id: textStatArea
                anchors.fill: parent

                text: "No data"

                readOnly: true
                wrapMode: TextArea.Wrap
            }
        }
    }

    Rectangle {
        id: myPathDrawer
        visible: true
        width: 360
        height: 270
        anchors.centerIn: parent
        anchors.topMargin: 300

        PathDrawer {
            pathData: [
                {"start": {"x": -100, "y": 0}, "end": {"x": 100, "y": 0}, "direction": "top", "angle": 0},
                {"start": {"x": 100, "y": 0}, "end": {"x": 100, "y": 100}, "direction": "right", "angle": 90},
                {"start": {"x": 100, "y": 1000}, "end": {"x": 1000, "y": 100}, "direction": "bottom", "angle": 180},
                {"start": {"x": 1000, "y": 100}, "end": {"x": -100, "y": -300}, "direction": "left", "angle": -90}
            ]
            anchors.fill: parent
            width: 420
            height: 300
        }
    }

    Row {
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: 310
        }
        spacing: 20

        Text {
            text: "(graphical)"
            color: "pink"
        }

        Slider {
            id: componentSlider
            width: 100
            from: 0
            to: 1
            stepSize: 1
            onValueChanged: {
                if (value === 0) {
                    myPathDrawer.visible = true
                    flickableContainer.visible = false
                } else {
                    myPathDrawer.visible = false
                    flickableContainer.visible = true
                }
            }
        }
        Text {
            text: "(textual)"
            color: "pink"
        }
    }


    Column {
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: 350
        }
        spacing: 10

        Row {
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
        Button {
            id: reset

            width: 300
            height: 50

            text: "Reset"

            onClicked: function () {
                // TODO: reset backend

                resetUI()
            }
        }

        Button { // TODO: remove this. the dialog should be apeared after end attempting
            id: authenticateBtn

            width: 300
            height: 50

            text: "Authenticate"

            onClicked: {
                motionBasedAuthentication.authenticate()
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

    function resetUI() {
        statTextShort.text = "Authentication result isn't available."
        statTextShort.color = "black"
    }
}


