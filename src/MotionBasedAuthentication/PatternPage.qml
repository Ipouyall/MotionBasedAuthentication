import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Controls.Material

Item {
    id: patternPage

    width: 480
    height: 600

    property var pathData: [
        {"start": {"x": 0, "y": 0}, "end": {"x": 100, "y": 0}, "direction": "top", "angle": 0},
        {"start": {"x": 100, "y": 0}, "end": {"x": 100, "y": 100}, "direction": "right", "angle": 90},
        {"start": {"x": 100, "y": 100}, "end": {"x": 0, "y": 100}, "direction": "bottom", "angle": 180},
        {"start": {"x": 0, "y": 100}, "end": {"x": 0, "y": 0}, "direction": "left", "angle": -90}
    ]

    anchors.centerIn: parent

    Canvas {
        id: patternMap
        width: 400
        height: 400
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);

            // Draw axes
            ctx.strokeStyle = "black";
            ctx.lineWidth = 1;
            ctx.beginPath();
            ctx.moveTo(40, 0);
            ctx.lineTo(40, height);
            ctx.moveTo(0, height - 40);
            ctx.lineTo(width, height - 40);
            ctx.stroke();

            // Draw path data
            ctx.strokeStyle = "blue";
            ctx.lineWidth = 3;
            ctx.beginPath();

            if (pathData.length > 0) {
                var scale = 1;
                var offsetX = 40;
                var offsetY = height - 40;
                var headln = 10;

                ctx.moveTo(offsetX + pathData[0].start.x * scale, offsetY - pathData[0].start.y * scale);
                for (var i = 0; i < pathData.length; i++) {
                    var startX = offsetX + pathData[i].start.x * scale;
                    var startY = offsetY - pathData[i].start.y * scale;
                    var endX = offsetX + pathData[i].end.x * scale;
                    var endY = offsetY - pathData[i].end.y * scale;

                    ctx.lineTo(endX, endY);
                    ctx.stroke();

                    // Draw arrowhead
                    drawArrow(ctx, startX, startY, endX, endY, headln);
                }
            }
            ctx.stroke();
        }
    }

    function drawArrow(ctx, fromX, fromY, toX, toY, headlen) {
        var angle = Math.atan2(toY - fromY, toX - fromX);

        ctx.beginPath();
        ctx.moveTo(toX, toY);
        ctx.lineTo(toX - headlen * Math.cos(angle - Math.PI / 6), toY - headlen * Math.sin(angle - Math.PI / 6));
        ctx.moveTo(toX, toY);
        ctx.lineTo(toX - headlen * Math.cos(angle + Math.PI / 6), toY - headlen * Math.sin(angle + Math.PI / 6));
        ctx.moveTo(toX, toY);
        ctx.stroke();
    }

    Button {
        text: "Back"
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
            bottomMargin: 10
        }
        width: 300
        height: 50

        onClicked: {
            stackView.pop()
        }
    }
}
