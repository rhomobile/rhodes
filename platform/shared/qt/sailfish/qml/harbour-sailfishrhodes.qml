import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"

ApplicationWindow
{
    initialPage: Component{FirstPage { }
    }
    cover: Component {
        Cover {
            transparent: true
            Label {
                anchors.centerIn: parent
                font.pixelSize: Theme.fontSizeLarge
                text: "Cover"
            }
        }
    }
    allowedOrientations: Orientation.All
}

