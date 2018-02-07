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
            id: delegate
            Label {
                id: deviceName
                x: Theme.horizontalPageMargin
                text: modelData.deviceName
                anchors.verticalCenter: parent.verticalCenter
                color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor

            }
            Label {
                x: Theme.horizontalPageMargin
                text: modelData.deviceId
                height: deviceName.height / 1.5
                anchors.verticalCenter: parent.verticalCenter
                color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
            }

            onClicked:{
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
