/*  FileReader.cpp

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

#include "FileReader.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>

FileReader::FileReader(QObject *parent) :
    QObject(parent),
    m_isValid()
{
    connect(this, &FileReader::filenameChanged, this, &FileReader::validateFilename);
}

void FileReader::validateFilename(const QString &filename)
{
    QFileInfo fi(filename);
    setIsValid(fi.exists() && fi.isFile());
}

bool FileReader::readFile()
{
    QFile f(m_filename);
    if (f.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&f);
        if (f.size() <= MAX_CONTENT_LEN) {
            setContent(in.readAll());

            return true;
        }
    }

    return false;
}

void FileReader::load(const QString& filename)
{
    setFilename(filename);
    validateFilename(filename);

    reload();
}

void FileReader::reload()
{
    if (m_isValid) {
        QFile f(m_filename);

        if (f.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&f);

            if (f.size() <= MAX_CONTENT_LEN) {
                setContent(in.readAll());
            }
        }
    }
}

void FileReader::setFilename(const QString &filename)
{
    if (filename != m_filename) {
        m_filename = filename;
        emit filenameChanged(m_filename);
    }
}

void FileReader::setIsValid(const bool isValid)
{
    if (isValid != m_isValid) {
        m_isValid = isValid;
        emit isValidChanged(isValid);
    }
}

void FileReader::setContent(const QString &content)
{
    if (content != m_content) {
        m_content = content;
        emit contentChanged();
    }
}
