import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.5

Page {

    Camera {
        id: camera
        focus.focusMode: Camera.FocusContinuous
        focus.focusPointMode: Camera.FocusPointAuto

    }

    Timer {
        interval: 100; running: true; repeat: true
        onTriggered: {
            barcodeModel[0].capture()

            okButton.visible = !barcodeModel[0].isActive
            repeakButton.visible = !barcodeModel[0].isActive
        }
    }



    Timer {
        interval: 300; running: true; repeat: true
        onTriggered: {
            if (textResult.text != barcodeModel[0].result){
                redLine.visible = !redLine.visible;
            }

            textResult.text = barcodeModel[0].result
        }
    }

    VideoOutput {
        id: videoOutput
        source: camera
        focus : visible // to receive focus and capture key events when visible
        anchors.fill: parent

        Rectangle{
            id: topRectangle
            anchors.top: parent.top
            width: parent.width
            height: (parent.height-parent.width)/2
            color: "black"
            opacity: 0.9
        }

        Rectangle{
            id: bottomRectangle
            anchors.bottom: parent.bottom
            width: parent.width
            height: (parent.height-parent.width)/2
            color: "black"
            opacity: 0.9
        }

        Rectangle{
            id: leftRectangle
            anchors.left: parent.left
            anchors.top: topRectangle.bottom
            anchors.bottom: bottomRectangle.top
            width: parent.width/20
            color: "black"
            opacity: 0.9
        }

        Rectangle{
            id: rightRectangle
            anchors.right: parent.right
            width: parent.width/20
            anchors.top: topRectangle.bottom
            anchors.bottom: bottomRectangle.top
            color: "black"
            opacity: 0.9
        }

        Rectangle{
            anchors.right: rightRectangle.left
            anchors.top: topRectangle.bottom
            width: parent.width/150
            height: (parent.height-parent.width)/2.5
            color: "red"
        }
        Rectangle{
            anchors.right: rightRectangle.left
            anchors.bottom: bottomRectangle.top
            width: parent.width/150
            height: (parent.height-parent.width)/2.5
            color: "red"
        }
        Rectangle{
            anchors.left: leftRectangle.right
            anchors.top: topRectangle.bottom
            width: parent.width/150
            height: (parent.height-parent.width)/2.5
            color: "red"
        }
        Rectangle{
            anchors.left: leftRectangle.right
            anchors.bottom: bottomRectangle.top
            width: parent.width/150
            height: (parent.height-parent.width)/2.5
            color: "red"
        }


        Rectangle{
            anchors.right: rightRectangle.left
            anchors.top: topRectangle.bottom
            width: (parent.height-parent.width)/2.5
            height: parent.width/150
            color: "red"
        }
        Rectangle{
            anchors.right: rightRectangle.left
            anchors.bottom: bottomRectangle.top
            width: (parent.height-parent.width)/2.5
            height: parent.width/150
            color: "red"
        }
        Rectangle{
            anchors.left: leftRectangle.right
            anchors.top: topRectangle.bottom
            width: (parent.height-parent.width)/2.5
            height: parent.width/150
            color: "red"
        }
        Rectangle{
            anchors.left: leftRectangle.right
            anchors.bottom: bottomRectangle.top
            width: (parent.height-parent.width)/2.5
            height: parent.width/150
            color: "red"
        }

        Rectangle{
            anchors.left: leftRectangle.right
            anchors.bottom: bottomRectangle.top
            width: (parent.height-parent.width)/2.5
            height: parent.width/150
            color: "red"
        }

        Rectangle{
            id: redLine
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width - leftRectangle.width - rightRectangle.width - parent.width/50
            height: parent.width/200
            opacity: 0.4
            color: "red"
        }


        Button{
            id: okButton
            width: 200
            height: 100
            x: parent.width - width - 50
            y: parent.height - 2*height
            z:1
            text: "OK"
            onClicked: {
                barcodeModel[0].done();
                pageStack.pop();
            }
        }

        Text {
            id: textResult
            elide: Text.ElideRight
            //anchors.verticalCenter: parent.verticalCenter
            y: (parent.height / 8)
            x: (parent.width - width) / 2
            color: "white"
        }


        Button{
            id: repeakButton
            width: 200
            height: 100
            x: 50
            y: parent.height - 2*height
            z:1
            text: "Retake"
            onClicked: {
                barcodeModel[0].restart();
            }
        }

    }
    on_NavigationChanged: {
        barcodeModel[0].cancel();
    }

}
