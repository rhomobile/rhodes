import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"

ApplicationWindow
{
    id: applicationWindow
    initialPage: FirstPageWK { }

    cover: Component {
        Cover {
            transparent: true
            anchors.fill: parent
            Image {
                source: "rho.png"
                fillMode: Image.PreserveAspectFit
                anchors.fill: parent
            }
            /*Label {
                anchors.centerIn: parent
                font.pixelSize: Theme.fontSizeLarge
                text: "Rho"
            }*/
        }
    }

    allowedOrientations: Orientation.All
    states: [
        State {
           name: "webEngine"
           PropertyChanges {
               target: applicationWindow
               initialPage: Component{FirstPageWE { }}
           }
        },
        State {
            name: "webKit"
            PropertyChanges {
                target: applicationWindow
                initialPage: Component{FirstPageWK { }}
            }
        }
    ]
    Component.onCompleted: {
        //applicationWindow.state = rootDelegate.webEngineEnabled ? "webEngine" : "webKit"
    }
}


