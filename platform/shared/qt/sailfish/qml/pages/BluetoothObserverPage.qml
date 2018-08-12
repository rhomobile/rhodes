import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page
    allowedOrientations: Orientation.All

    SilicaListView {
        id: listView
        model: bluetoothListModel

        anchors.fill: parent
        header: PageHeader {
            title: "Available bluetooth devices"
        }


        delegate: BackgroundItem {
            height: 150
            width: Screen.width

            Label {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                id: deviceName
                //x: Theme.horizontalPageMargin
                text: "   " + modelData.deviceName + "\n   " + modelData.deviceId
                color: Theme.primaryColor
                anchors.top: parent.top

            }

            onClicked:{
                console.log("Clicked on " + deviceName)
                modelData.clicked();
                pageStack.pop();
            }



        }

        VerticalScrollDecorator {}

    }
    on_NavigationChanged: {
        bluetoothModel[0].cancel();
    }
}
