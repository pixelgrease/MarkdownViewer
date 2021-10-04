/*  FileWatcher.cpp

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

#include "FileWatcher.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>

FileWatcher::FileWatcher(QObject *parent) :
    QObject(parent),
    m_isValid()
{
    connect(&m_watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::changed);
    connect(this, &FileWatcher::filenameChanged, this, &FileWatcher::monitorFile);
}

void FileWatcher::setUrl(QUrl url)
{
    setFilename(url.toLocalFile());
}

void FileWatcher::validateFilename(const QString &filename)
{
    QFileInfo fi(filename);
    setIsValid(fi.exists() && fi.isFile());
}

void FileWatcher::monitorFile(const QString &filename)
{
    // only watch one file
    const auto& files = m_watcher.files();
    if (files.length()) {
        m_watcher.removePaths(files);
    }

    validateFilename(filename);

    if (m_isValid) {
        m_watcher.addPath(filename);
        emit changed(filename);
    }
}

void FileWatcher::setFilename(const QString &filename)
{
    if (filename != m_filename) {
        m_filename = filename;
        emit filenameChanged(m_filename);
    }
}

void FileWatcher::setIsValid(const bool isValid)
{
    if (isValid != m_isValid) {
        m_isValid = isValid;
        emit isValidChanged(isValid);
    }
}
