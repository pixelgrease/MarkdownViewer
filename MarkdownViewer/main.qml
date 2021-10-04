/*  main.qml

    The FileReader class simplifies reading text files for QML

    Copyright (c) 2020 Stan Morris

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.14
import OCQt 1.0

ApplicationWindow {
    property bool isLoaded: preview.text.length > 0

    function isValidURL(str) {
       var regexp = /(ftp|http|https):\/\/(\w+:{0,1}\w*@)?(\S+)(:[0-9]+)?(\/|\/([\w#!:.?+=&%@!\-\/]))?/
       return regexp.test(str);
    }
    function removeQuotes(str) {
        return str.replace(/['"]+/g, '')
    }

    visible: true
    width: 640
    height: 960
    title: watcher.filename ? watcher.filename
                            : qsTr("Markdown Viewer - drag a file here to view")
    footer: ToolBar {
        enabled: isLoaded
        RowLayout {
            id: row
            readonly property real padding: statusLbl.height
            anchors { fill: parent; leftMargin: padding; rightMargin: padding }
            Label {
                id: statusLbl
                elide: Label.ElideMiddle
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
            }
            ToolButton {
                text: "F5"
                enabled: reader.isValid
                onClicked: reader.reload()
            }
            Slider {
                id: sizeSlider
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: 8*row.padding
                from: .5; to: 4.0
                stepSize: .05
                value: 1
                clip: true
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: da.containsDrag     ? "deepskyblue" :
               preview.text.length ? "white"
                                   : "gray"
    }

    DropArea {
        id: da
        function handleDropped(drop) {
            if (drop.hasUrls) {
                loadUrl(drop.urls[0]);
            } else if (drop.hasText) {
                preview.text = drop.text;
            }
        }

        function loadUrl(url) {
            try {
                watcher.setUrl(url);
            } catch(e) {
                console.error(e)
            }

            preview.text = Qt.binding(function(){ return reader.content; });
        }

        anchors.fill: parent
        onDropped: handleDropped(drop)
    }

    Flickable {
        anchors.fill: parent
        contentWidth: width; contentHeight: previewWrapper.height
        focus: true
        ScrollBar.vertical: ScrollBar {
            width: 20
            policy: ScrollBar.AlwaysOn
        }
        Keys.onPressed: {
            console.log("key F5:", Qt.Key_F5, ", pressed:", event.key);
            if (event.key === Qt.Key_F5) {
                reader.reload();
            }
        }

        Item {
            id: previewWrapper
            anchors { left: parent.left; right: parent.right; rightMargin: 20 }
            height: preview.height*preview.scale

            Label {
                id: preview
                anchors.centerIn: parent
                width: parent.width/sizeSlider.value
                scale: sizeSlider.value
                wrapMode: Text.Wrap
                textFormat: Text.MarkdownText
                text: reader.content
                onLinkActivated: if (isValidURL(link)) { Qt.openUrlExternally(removeQuotes(link)) }
                onHoveredLinkChanged: statusLbl.text = removeQuotes(hoveredLink)
                lineHeight: 1.2
                color: "#444"

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.NoButton
                    cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                }

                FileWatcher { id: watcher; onChanged: reader.load(filename) }
                FileReader { id: reader }
            }
        }
    }
    Label {
        id: messages
        anchors { fill: parent; margins: 20 }
        horizontalAlignment: Label.AlignHCenter
        verticalAlignment: Label.AlignVCenter
        font.pointSize: 24
        visible: !isLoaded
        text: "Drop a .md file to load it"
    }

    Timer {
        id: delayedLoader
        interval: 1
        running: OptionalCommandlineArgument ? true : false
        onTriggered: watcher.filename = OptionalCommandlineArgument
    }
}
