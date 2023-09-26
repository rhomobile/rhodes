import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"

ApplicationWindow
{
    id: applicationWindow

    Loader {
       id: firstPage
       source: "pages/FirstPageWK.qml"
    }

    initialPage: firstPage.item

    cover: Component {
        Cover {
            transparent: true
            anchors.fill: parent
            Image {
                source: "rho.png"
                fillMode: Image.PreserveAspectFit
                anchors.fill: parent
            }
        }
    }

    allowedOrientations: Orientation.All

}
