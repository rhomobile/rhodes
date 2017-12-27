import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    SilicaListView {
        id: listView
        model: mainMenuListModel

        anchors.fill: parent
        header: PageHeader {
            title: "Menu"
        }


        delegate: BackgroundItem {
            id: delegate

            Label {
                x: Theme.horizontalPageMargin
                text: modelData.text
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
}
