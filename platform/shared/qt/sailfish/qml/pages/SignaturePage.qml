import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    Canvas {
        id: canvas
        x: 50
        y: 50
        width: parent.width - 100
        height: parent.height - 350


        property real lastX
        property real lastY
        property color color: "black"


        onPaint: {
            var ctx = getContext('2d')
            ctx.lineWidth = 1.5
            ctx.strokeStyle = canvas.color
            ctx.beginPath()
            ctx.moveTo(lastX, lastY)
            lastX = area.mouseX
            lastY = area.mouseY
            ctx.lineTo(lastX, lastY)
            ctx.stroke()

        }

        function fillWhite(){
            var ctx = canvas.getContext("2d");
            ctx.fillStyle = Qt.rgba(1, 1, 1, 1);
            ctx.fillRect(0, 0, canvas.width, canvas.height);
            canvas.requestPaint();
        }

        MouseArea {
            id: area
            anchors.fill: parent
            onPressed: {
                canvas.lastX = mouseX
                canvas.lastY = mouseY
            }
            onPositionChanged: {
                canvas.requestPaint()
            }
        }


    }


    Timer {
        interval: 300; running: true; repeat: false
        onTriggered: {
            canvas.fillWhite();
        }
    }

    Rectangle {
        id: okButton
        width: 100
        height: 100
        x: parent.width - 2*width
        y: parent.height - 2*height
        color: (scale > 1)?"#00ff00":"#20ff20"
        radius: 50
        scale: captureMouseArea.pressed ? 1.5 : 1

        Text {
            anchors.centerIn: parent
            font.pixelSize: 20
            color: "black"
            text: "Ok"
        }

        MouseArea {
            id: captureMouseArea
            anchors.fill: parent;
            onClicked: {
                canvas.save(signatureModel[0].fileName);
                console.log(signatureModel[0].fileName);
                signatureModel[0].done();
                pageStack.pop();
            }

        }

     }

    Rectangle {
        id: cleanButton
        width: 100
        height: 100
        x: width
        y: parent.height - 2*height
        color: (scale > 1)?"#ff00ff":"#ff20ff"
        radius: 50
        scale: cleanMouseArea.pressed ? 1.5 : 1

        Text {
            anchors.centerIn: parent
            font.pixelSize: 20
            color: "black"
            text: "Clean"
        }

        MouseArea {
            id: cleanMouseArea
            anchors.fill: parent;
            onClicked: {
                canvas.fillWhite();
            }
        }


    }

    on_NavigationChanged: {
        signatureModel[0].cancel();
    }

}
