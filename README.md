# Markdown Viewer

Drop-target QML app for showing markdown documents, [source code available on github](https://github.com/pixelgrease/MarkdownViewer).

---

Gloriously demonstrates:
 
* **DropArea** - used as a drop-target for text files with markdown formatting
* **Text** - standard QML component displays markdown and opens clicked links in browser
* **FileWatcher** - a C++ component for QML using QFileSystemWatcher to detect file changes
* **FileReader** - a C++ component for QML which simplifies reading text files.
 
---

## FileWatcher

The FileWatcher class monitors file signals when a file is modified.  
- **Properties**  

  `filename` - the name of file to watch  
  `isValid` - true if watched file exists  

- **Signals**

  `changed` - signals that *filename* property is modified or that its content changed

- **Usage**

  This snippet will display updated markdown each time the file is changed on disk:

  ```
  Text {
      textFormat: Text.MarkdownText
      text: reader.content

      FileWatcher {
          filename: "C:/Dev/MarkdownViewer/README.md"
          onChanged: reader.load(filename)
      }
      FileReader { id: reader }
  }
  ```

---

## FileReader

The FileReader class simplifies reading text files for QML.  

* **Properties**  

  `filename` - the name of the file to watch
  `isValid` - indicates that the watched filen exists  

* **Signals**

  `changed` - this signal is emitted when the filename is set or changed or when the content changes  

* **Usage**

  See the example above.

---

## Changes

2021-10-03 **Bug Fix and minor improvements**
* Incorporates workaround for [QTBUG-97041 (text with backticks doesn't scale properly)](https://bugreports.qt.io/browse/QTBUG-97041) so that code scales correctly
* Supports flicking the document to scroll
* Increases width of scrollbar and no longer draw scrollbar over text
* Handles dropped files on Linux
* Handles dropped markdown text
* Adds application icon

2020-04-01 **Functionality Updates**
* Loads file specified on command-line, enabling launching associated file type (ex.: *.md) using a file manager
* Clicking link loads link in browser 
* Show name of file in title bar
* Sets an application icon [based on design](https://github.com/dcurtis/markdown-mark) created by [Dustin Curtis](https://dustincurtis.com)
  
  
2020-02-17 **Initial Version**
* Presented at OC Qt meetup in demonstration of Markdown support in Qt 5.14

---

## Known Defects
* Does not update for changes on remote file system
  1. open file on network driver
  2. open same file in file editor
  3. modify file

     observe: file does not update

* Locks file
  1. open file
  2. use Windows Explorer to move file

     observe: file cannot be moved

         note: could not reproduce in **C:\Program Files\MarkdownViewer** folder, but rights were elevated.
