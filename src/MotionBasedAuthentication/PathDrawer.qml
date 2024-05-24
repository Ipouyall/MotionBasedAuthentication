import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Controls.Material

Item {
    id: pathDrawer

    property var pathData: []

    width: 420
    height: 400

    Canvas {
        id: patternMap
        width: parent.width - 20
        height: parent.height - 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.top
        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);

            // Adjust pathData to ensure all coordinates are positive
            var minX = findMin('x', pathData);
            var minY = findMin('y', pathData);
            var adjustedPathData = adjustPathData(minX, minY);

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

            // Calculate scale to fit within canvas
            var maxY = findMax('y', adjustedPathData);
            var maxX = findMax('x', adjustedPathData);
            var scaleX = Math.min(width*0.8, width-80) / maxX;
            var scaleY = Math.min(height*0.8, height-80) / maxY;
            var scale = Math.min(scaleX, scaleY, 1);

            if (adjustedPathData.length > 0) {
                var offsetX = 40;
                var offsetY = height - 40;
                var headln = 10;

                ctx.moveTo(offsetX + adjustedPathData[0].start.x * scale, offsetY - adjustedPathData[0].start.y * scale);
                for (var i = 0; i < adjustedPathData.length; i++) {
                    var startX = offsetX + adjustedPathData[i].start.x * scale;
                    var startY = offsetY - adjustedPathData[i].start.y * scale;
                    var endX = offsetX + adjustedPathData[i].end.x * scale;
                    var endY = offsetY - adjustedPathData[i].end.y * scale;

                    ctx.lineTo(endX, endY);
                    ctx.stroke();

                    // Draw arrowhead
                    drawArrow(ctx, startX, startY, endX, endY, headln);
                }
            }
            ctx.stroke();
        }

        Component.onCompleted: patternMap.requestPaint()
    }

    function requestPaint(path) {
        pathData = path
        console.log("(1) Length:", path.length)
        for (var i = 0; i < path.length; i++)
            console.log("(1) [",i,"]: ", path[i])
        patternMap.requestPaint()
    }

    function findMin(axis, array) {
        var min = Number.MAX_VALUE;
        for (var i = 0; i < array.length; i++) {
            min = Math.min(min, array[i].start[axis], array[i].end[axis]);
        }
        return min;
    }

    function adjustPathData(minX, minY) {
        var adjusted = [];
        for (var i = 0; i < pathData.length; i++) {
            var startX = pathData[i].start.x - minX;
            var startY = pathData[i].start.y - minY;
            var endX = pathData[i].end.x - minX;
            var endY = pathData[i].end.y - minY;
            adjusted.push({
                "start": {"x": startX, "y": startY},
                "end": {"x": endX, "y": endY},
            });
        }
        return adjusted;
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

    function findMax(axis, array) {
        var max = Number.MIN_VALUE;
        for (var i = 0; i < array.length; i++) {
            max = Math.max(max, array[i].start[axis], array[i].end[axis]);
        }
        return max;
    }
}
