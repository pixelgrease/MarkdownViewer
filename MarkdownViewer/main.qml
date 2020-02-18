import QtQuick 2.14
import QtQuick.Window 2.14
import OCQt 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Markdown Viewer")

    Rectangle {
        anchors.fill: parent
        color: da.containsDrag ? "deepskyblue" :
               watcher.isValid ? "white"
                               : "gray"
    }

    DropArea {
        id: da
        function handleDropped(drop) {
            if (drop.hasText) {
                var path = drop.text;
                path = path.replace(/^(file:\/{3})/,"");

                watcher.filename = decodeURIComponent(path);
            }
        }

        anchors.fill: parent
        onDropped: handleDropped(drop)
    }

    Item {
        anchors { fill: parent; margins: 5 }

        Text {
            id: preview
            anchors.fill: parent
            wrapMode: Text.WrapAnywhere
            textFormat: Text.MarkdownText
            text: reader.content

            FileWatcher { id: watcher; onChanged: reader.load(filename) }
            FileReader { id: reader }
        }
    }
}
