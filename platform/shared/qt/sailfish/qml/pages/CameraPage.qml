import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.5

Page {
    Camera {
        id: camera

        imageCapture {
            id: capturer
            onImageSaved: {
                camerasModel[0].captured(path);
                pageStack.pop();
            }
        }
    }

    VideoOutput {
        id: videoOutput
        source: camera
        focus : visible // to receive focus and capture key events when visible
        anchors.fill: parent


        Rectangle {
            id: captureButton
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
                text: "Capture"
            }

            MouseArea {
                id: captureMouseArea
                anchors.fill: parent;
                onClicked: {
                    camera.imageCapture.capture();
                }

            }

         }

        Rectangle {
            id: cancelButton
            width: 100
            height: 100
            x: width
            y: parent.height - 2*height
            color: (scale > 1)?"#ff0000":"#ff2020"
            radius: 50
            scale: cancelMouseArea.pressed ? 1.5 : 1

            Text {
                anchors.centerIn: parent
                font.pixelSize: 20
                color: "black"
                text: "Cancel"
            }

            MouseArea {
                id: cancelMouseArea
                anchors.fill: parent;
                onClicked: {
                    camerasModel[0].captured("");
                    pageStack.pop();
                }
            }


        }
    }
    on_NavigationChanged: {
        camerasModel[0].captured("");
    }

}
