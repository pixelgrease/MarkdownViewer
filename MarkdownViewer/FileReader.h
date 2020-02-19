#pragma once
/*  FileReader.h

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

#include <QObject>
#include <QFileSystemWatcher>
#include <QUrl>

class FileReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filename  READ filename                    NOTIFY filenameChanged)
    Q_PROPERTY(bool isValid      READ isValid   WRITE setIsValid  NOTIFY isValidChanged)
    Q_PROPERTY(QString content   READ content                     NOTIFY contentChanged)

public:
    explicit FileReader(QObject *parent = nullptr);

signals:
    void changed(const QString& filename);
    void filenameChanged(const QString& filename);
    void isValidChanged(bool isValid);
    void contentChanged();

public slots:
    void load(const QString& filename);

private slots:
    void validateFilename(const QString& filename);

private:
    friend class tst_FileReader;

    static const int MAX_CONTENT_LEN = 65536;   // arbitrary 64K limit

    QString filename() const                    { return m_filename; }
    void setFilename(const QString& filename);
    bool isValid() const                        { return m_isValid; }
    void setIsValid(const bool isValid);
    QString content() const                     { return m_content; }
    void setContent(const QString& content);
    bool readFile();

    QString m_filename;
    bool m_isValid;
    QString m_content;
};

