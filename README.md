# MarkdownViewer
Drop-target QML app which shows content of markdown document smaller than 64K.  
<br/>
## Overview
This project was created to drive development on a utility class, FileWatcher, which signals when the watched file is modified.  
<br/>
  
### class FileWatcher
The FileWatcher class monitors changes to a single file and signals when the file is modified.  
<br/>
#### Properties
  `filename` - the name of the file to watch  
  `isValid` - indicates that the watched filen exists  
<br/>

#### Signals
`changed` - this signal is emitted when the filename is set or changed or when the content changes  
<br/>

### class FileReader
The FileReader class simplifies reading text files for QML.  
<br/>
#### Properties
  `filename` - the name of the file to watch
  `isValid` - indicates that the watched filen exists  
<br/>

#### Signals
  `changed` - this signal is emitted when the filename is set or changed or when the content changes  
<br/>
