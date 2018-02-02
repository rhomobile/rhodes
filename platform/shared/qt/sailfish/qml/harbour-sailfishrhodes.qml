import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"

ApplicationWindow
{

    initialPage: Component{FirstPage { }}
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
}


