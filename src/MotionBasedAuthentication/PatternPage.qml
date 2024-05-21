import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Controls.Material

Item {
    id: patternPage

    width: 400
    height: 400

    property var path: [
        {"start": {"x": 0, "y": 0}, "end": {"x": 100, "y": 0}, "direction": "top", "angle": 0},
        {"start": {"x": 100, "y": 0}, "end": {"x": 100, "y": 100}, "direction": "right", "angle": 90},
        {"start": {"x": 100, "y": 100}, "end": {"x": 0, "y": 100}, "direction": "bottom", "angle": 180},
        {"start": {"x": 0, "y": 100}, "end": {"x": 0, "y": 0}, "direction": "left", "angle": -90}
    ]

    anchors.centerIn: parent

    Canvas {
        id: canvas
        width: 400 * 0.8
        height: 400 * 0.8
        anchors.centerIn: parent

        onPaint: {
            var ctx = canvas.getContext("2d")
            ctx.clearRect(0, 0, canvas.width, canvas.height)

            ctx.strokeStyle = "blue"
            ctx.lineWidth = 2

            for (var i = 0; i < path.length; i++) {
                var segment = path[i]
                ctx.beginPath()
                ctx.moveTo(segment.start.x, segment.start.y)
                ctx.lineTo(segment.end.x, segment.end.y)
                ctx.stroke()
            }
        }
    }

    Button {
        text: "Back"
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
            bottomMargin: 20
        }
        width: 300
        height: 50

        onClicked: {
            stackView.pop()
        }
    }
}
