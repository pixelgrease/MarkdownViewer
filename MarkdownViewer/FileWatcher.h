#pragma once
/*  FileWatcher.h

    The FileWatcher class monitors changes to a file and signals when it is modified

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

#include <QObject>
#include <QFileSystemWatcher>
#include <QUrl>

class FileWatcher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filename  READ filename  WRITE setFilename  NOTIFY filenameChanged)
    Q_PROPERTY(bool isValid      READ isValid   WRITE setIsValid   NOTIFY isValidChanged)

public:
    explicit FileWatcher(QObject *parent = nullptr);

signals:
    void changed(const QString& filename);
    void filenameChanged(const QString& filename);
    void isValidChanged(bool isValid);

private slots:
    void validateFilename(const QString& filename);
    void monitorFile(const QString& filename);

private:
    friend class tst_FileWatcher;

    static const int MAX_CONTENT_LEN = 65536;   // arbitrary 64K limit

    QString filename() const                    { return m_filename; }
    void setFilename(const QString& filename);
    bool isValid() const                        { return m_isValid; }
    void setIsValid(const bool isValid);

    QFileSystemWatcher m_watcher;
    QString m_filename;
    bool m_isValid;
};

