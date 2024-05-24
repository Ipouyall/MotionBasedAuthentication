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

        onPathChanged: function(path) {
            myPathDrawer.pathData = path
            pathDrawerComponent.requestPaint(path)
        }
    }

    Label {
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: 10
        }
        id: statTextShort
        width: 400
        height: 50

        text: "Authentication result isn't available"
        horizontalAlignment: Qt.AlignCenter
    }

    Rectangle {
        id: flickableContainer
        visible: true
        width: parent.width * 0.75
        height: parent.height * 0.45
        anchors.centerIn: parent
        anchors.topMargin: parent.height * 0.5

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
        visible: false
        width: parent.width * 0.75
        height: parent.height * 0.45
        anchors.centerIn: parent
        anchors.topMargin: parent.height * 0.5
        property variant pathData: []

        PathDrawer {
            id: pathDrawerComponent
            pathData: myPathDrawer.pathData
            anchors.fill: parent
            width: parent.width
            height: parent.height
        }
    }

    Row {
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: parent.height * 0.517
        }
        spacing: 20

        Text {
            text: "(textual)"
            color: "pink"
        }

        Slider {
            id: componentSlider
            width: 100
            from: 0
            to: 1
            stepSize: 1
            onValueChanged: {
                if (value === 1) {
                    myPathDrawer.visible = true
                    flickableContainer.visible = false
                } else {
                    myPathDrawer.visible = false
                    flickableContainer.visible = true
                }
            }
        }
        Text {
            text: "(graphical)"
            color: "pink"
        }
    }


    Column {
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: parent.height * 0.584
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
                // motionBasedAuthentication.authenticate()
                motionBasedAuthentication.getPath()
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


