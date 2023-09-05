import QtQuick 2.0
import Sailfish.Silica 1.0
import QtWebKit 3.0
import QtMultimedia 5.5
import QtWebKit.experimental 1.0
import Nemo.KeepAlive 1.2

Page {
    id: mainPage
    Image{
        id: image
        source: rootDelegate.cover
        visible: rootDelegate.cover !== ""
        anchors.top: menuButton.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }

    KeepAlive {
        id: keepAlive
        enabled: true;
    }

    Rectangle {
        width: parent.itemWidth
        height: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        color: "black"
        id: menuButton
        MouseArea {
            anchors.fill: menuButton
            onClicked: {
                pageStack.push(Qt.resolvedUrl("MenuPage.qml"))
            }
        }
    }

    Rectangle {
        visible: rootDelegate.cover === ""
        id: fickable
        anchors.top: menuButton.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

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

                WebView {
                    id: webView
                    url: modelData.url
                    anchors.fill: parent
                    experimental.preferredMinimumContentsWidth: Screen.width / modelData.scale
                    experimental.customLayoutWidth: Screen.width / modelData.scale


                    Connections {
                        target: modelData
                        onSetHtml: {
                            modelData.messageReceived("Setting HTML");
                            webView.loadHtml(pHtml);
                        }
                        onGoBack:{
                            modelData.messageReceived("Going back");
                            webView.goBack();
                        }
                        onGoForward:{
                            modelData.messageReceived("Going forward");
                            webView.goForward();
                        }
                        onRefresh:{
                            modelData.messageReceived("Reloading page");
                            webView.reload();
                        }
                        onEvaluateJavaScript:{
                            modelData.messageReceived("Trying to evaluate JS");
                            webView.experimental.evaluateJavaScript(pScript)
                        }
                        onSetCurrentTab:{
                            modelData.messageReceived("Trying to set current tab: don't realized");
                        }
                        onSwitchToThisTab:{
                            modelData.messageReceived("Trying to switch to this tab: don't realized");
                        }
                        onOpenQMLDocument:{
                            pageStack.push(Qt.resolvedUrl(documentName));
                        }
                    }

                    onNavigationRequested: {
                        modelData.messageReceived("onNavigationRequested " + request.url + " : " + request.action)
                    }

                    onLoadingChanged: {
                        if (loadRequest.status == WebView.LoadStartedStatus){
                            modelData.loadStarted();
                            modelData.messageReceived("Loading " + url + " started...");
                        }
                        if (loadRequest.status == WebView.LoadSucceededStatus){
                            rootDelegate.cover = ""
                            modelData.loadFinished(true);
                            modelData.messageReceived("Page " + url + " loaded!");
                        }
                        if (loadRequest.status == WebView.LoadFailedStatus){
                            modelData.loadFinished(false);
                            modelData.messageReceived("Loading error: " + url + " : "+
                                                      loadRequest.errorCode.toString() + " " + loadRequest.errorString);
                        }
                    }
                    onLinkHovered: {
                        modelData.messageReceived("Link clicked: " + hoveredUrl);
                        modelData.linkClicked(hoveredUrl)
                    }
                    Component.onCompleted: {
                        modelData.messageReceived("Component complited")
                    }
                    //experimental.preferences.javascriptEnabled: true;
                    //experimental.preferences.fileAccessFromFileURLsAllowed: true;
                    //experimental.preferences.webGLEnabled: true;
                    experimental.preferences.navigatorQtObjectEnabled: true;
                    experimental.preferences.offlineWebApplicationCacheEnabled: false;
                    experimental.onMessageReceived: {
                        modelData.messageReceived(message.data);
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
