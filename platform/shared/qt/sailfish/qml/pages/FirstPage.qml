import QtQuick 2.0
import Sailfish.Silica 1.0
import QtWebEngine 1.0
import QtMultimedia 5.5
//import Nemo.KeepAlive 1.1

Page {
    id: mainPage
    Image{
        id: image
        source: rootDelegate.cover
        visible: rootDelegate.cover !== ""
        anchors.fill: parent
    }
    /*
    KeepAlive {
        id: keepAlive
        enabled: true;
    }
    */
    SilicaFlickable {
        visible: rootDelegate.cover === ""
        id: fickable
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: "Menu"
                onClicked: pageStack.push(Qt.resolvedUrl("MenuPage.qml"))
            }
            MenuItem {
                text: "Exit"
                onClicked: pageStack.push(Qt.quit())
            }
        }

        Row {
            id: tabHeader
            anchors.top: parent.top
            width: parent.width
            height: (mainView.count > 1) ? 100 : 1
            visible: (mainView.count > 1) ? true : false
            opacity: 0.98
            Repeater {
                model: webViewsModel

                anchors.fill: parent
                Rectangle {
                    color: "white"

                    height: parent.height
                    width: parent.width / ((mainView.count > 1) ? mainView.count : 1)

                    Label {
                        anchors.centerIn: parent
                        text: modelData.text
                        color: "black"//Theme.highlightColor
                        font {
                            bold: true
                            pixelSize: Theme.fontSizeExtraSmall
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var selectedIndex = Math.round(parent.x/parent.width);
                            console.log("selected index: ", selectedIndex)
                            console.log("mainView.currentIndex: ", mainView.currentIndex)
                            while (selectedIndex != mainView.currentIndex) {
                                mainView.incrementCurrentIndex()
                            }

                        }
                    }
                }
            }
        }

        SlideshowView {
            id: mainView
            clip:true
            model: webViewsModel
            itemWidth: width
            itemHeight: height
            //onCurrentIndexChanged: {}

            anchors {
                top: tabHeader.bottom
                left: parent.left
                right: parent.right
                bottom: toolBar.top
            }

            delegate:  Rectangle {
                width: parent.itemWidth
                height: parent.height
                border.width: 1

                WebEngineView {
                    id: webView
                    url: modelData.url
                    anchors.fill: parent
                    //zoomFactor: modelData.scale
                    visible: true

                    Connections {
                        target: modelData
                        onSetHtml: {
                            console.log("Setting HTML");
                            webView.loadHtml(pHtml);
                        }
                        onGoBack:{
                            console.log("Going back");
                            webView.goBack();
                        }
                        onGoForward:{
                            console.log("Going forward");
                            webView.goForward();
                        }
                        onRefresh:{
                            console.log("Reloading page");
                            webView.reload();
                        }
                        onEvaluateJavaScript:{
                            console.log("Trying to evaluate JS");
                            webView.runJavaScript(pScript)
                        }
                        onSetCurrentTab:{
                            console.log("Trying to set current tab: don't realized");
                        }
                        onSwitchToThisTab:{
                            console.log("Trying to switch to this tab: don't realized");
                        }
                        onOpenQMLDocument:{
                            pageStack.push(Qt.resolvedUrl(documentName));
                        }
                    }


                    onNavigationRequested: {
                        console.log(request.url)
                        console.log(request.action)
                    }

                    onLoadingChanged: {
                        if (loadRequest.status == WebEngineView.LoadStartedStatus){
                            modelData.loadStarted();
                            console.log("Loading " + url + " started...");
                        }
                        if (loadRequest.status == WebEngineView.LoadSucceededStatus){
                            modelData.loadFinished(true);
                            console.log("Page " + url + " loaded!");
                        }
                        if (loadRequest.status == WebEngineView.LoadFailedStatus){
                            modelData.loadFinished(false);
                            console.log("Page " + url + " loaded with fail: " +
                                        loadRequest.errorCode + " " + loadRequest.errorString);
                            modelData.messageReceived("Loading error: " + url + " : "+ loadRequest.errorCode + " " + loadRequest.errorString);
                        }
                    }
                    onLinkHovered: {
                        console.log("Link clicked: " + hoveredUrl);
                        modelData.linkClicked(hoveredUrl)
                    }
                    Component.onCompleted: {
                        console.log("Component complited")
                    }


                    onJavaScriptConsoleMessage: {
                        modelData.messageReceived(message);
                    }

                }
            }
        }

        Row {
            id: toolBar
            anchors{
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
            width: parent.width
            height: (repeater.count > 0) ? 100 : 1
            visible: (repeater.count > 0) ? true : false
            Repeater {
                id: repeater
                model: toolBarModel
                anchors.fill: parent
                Rectangle {
                    id: toolBarItem
                    color: "grey"
                    opacity: 0.5
                    height: parent.height
                    width: parent.width / 2
                    Label {
                        anchors.centerIn: parent
                        text: modelData.text
                        color: Theme.highlightColor
                        opacity: 1
                        font {
                            bold: true
                            pixelSize: Theme.fontSizeExtraSmall
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var selectedIndex = Math.round(parent.x/parent.width);
                            console.log("selected index: ", selectedIndex)
                            toolBarModel[selectedIndex].clicked()
                        }
                    }
                }
            }
        }

    }
}
