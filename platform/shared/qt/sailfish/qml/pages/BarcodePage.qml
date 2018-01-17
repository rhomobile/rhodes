import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.5

Page {
    Camera {
        id: camera
    }

    Timer {
        interval: 40; running: true; repeat: true
        onTriggered: {
            barcodeModel[0].capture()
            okButton.visible = !barcodeModel[0].isActive
            repeakButton.visible = !barcodeModel[0].isActive
        }
    }


    Timer {
        interval: 300; running: true; repeat: true
        onTriggered: {
            textResult.text = barcodeModel[0].result
        }
    }

    VideoOutput {
        id: videoOutput
        source: camera
        focus : visible // to receive focus and capture key events when visible
        anchors.fill: parent
        MouseArea {
            anchors.fill: parent;
            onClicked: camera.FocusAreaFocused
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
                    barcodeModel[0].done();
                    pageStack.pop();
                }

            }

         }


        Text {
            id: textResult
            //anchors.verticalCenter: parent.verticalCenter
            y: (parent.height / 4) * 3
            x: (parent.width - width) / 2
            color: "white"
        }

        Rectangle {
            id: repeakButton
            width: 100
            height: 100
            x: width
            y: parent.height - 2*height
            color: (scale > 1)?"#ff00ff":"#ff20ff"
            radius: 50
            scale: cancelMouseArea.pressed ? 1.5 : 1

            Text {
                anchors.centerIn: parent
                font.pixelSize: 20
                color: "black"
                text: "Repeak"
            }

            MouseArea {
                id: cancelMouseArea
                anchors.fill: parent;
                onClicked: {
                    barcodeModel[0].restart();
                }
            }


        }
    }
    on_NavigationChanged: {
        barcodeModel[0].cancel();
    }

}
