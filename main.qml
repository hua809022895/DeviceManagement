import QtQuick 2.6
import QtQuick.Window 2.2
import QtWebView 1.1

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    WebView {
        id: webView
        anchors.fill: parent
        url: "file:///D:/SRTM/huayin14/index.html"
        onLoadingChanged: {
            if (loadRequest.errorString)
                console.error(loadRequest.errorString);
        }
    }
}
