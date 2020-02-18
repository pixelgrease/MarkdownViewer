# MarkdownViewer

Drop-target QML app which shows content of markdown document smaller than 64K.  

The following technologies are demnstrated: 
 
* **DropArea** - the QML component is used as a drop-target for text files with markdown formatting
* **FileWatcher** - a QML component implemented in C++ using the QFileSystemWatcher class to notify QML when a file is modified
* **FileReader** - a QML component implemented in C++ which simplifies reading text files.


## FileWatcher

The FileWatcher class monitors changes to a single file and signals when the file is modified.  
<br/>
* **Properties**  

  `filename` - the name of the file to watch  
  `isValid` - indicates that the watched filen exists  


* **Signals**

  `changed` - this signal is emitted when the filename is set or changed or when the content changes  

* **Usage**

  The following snippet will show the modified markdown-formatted file each time it is saved:

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

## FileReader

The FileReader class simplifies reading text files for QML.  

* **Properties**  

  `filename` - the name of the file to watch
  `isValid` - indicates that the watched filen exists  

* **Signals**

  `changed` - this signal is emitted when the filename is set or changed or when the content changes  

* **Usage**

  See the example above.
