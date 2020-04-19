/*  tst_FileReader.cpp

    Unit tests for the FileReader class

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
#include <QSignalSpy>
#include <QtTest>

// add necessary includes here

class tst_FileReader : public QObject
{
    Q_OBJECT

public:
    tst_FileReader();
    ~tst_FileReader();

private slots:
    void initialization();
    void filename_member();
    void isValid_with_invalid_folder();
    void isValid_with_invalid_file();
    void isValid();
    void load_file();
    void reload_file();

private:
    QDir makeInvalidDir();
    void makeTemporaryFile(QFile& file, const QString &filename);

    QTemporaryDir m_tempDir;
    const QString defaultContent;
};

tst_FileReader::tst_FileReader() : defaultContent("hello, world!")
{
    const QString tempDirPattern = QDir::cleanPath(QDir::tempPath() +
                                                   QDir::separator() +
                                                   "tst_FileReader");
    QTemporaryDir tempDir(tempDirPattern);
}

tst_FileReader::~tst_FileReader()
{
}

QDir tst_FileReader::makeInvalidDir()
{
    QTemporaryDir tempDir;
    const QDir dir(tempDir.path());
    tempDir.remove();
    return dir;
}

void tst_FileReader::makeTemporaryFile(QFile& file, const QString &filename)
{
    file.setFileName(m_tempDir.path() + QDir::separator() + filename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.write(QByteArray(qPrintable(defaultContent)));
    file.close();
}

void tst_FileReader::initialization()
{
    FileReader fr;
    QVERIFY(fr.m_filename.isEmpty());
}

void tst_FileReader::filename_member()
{
    const QString FILENAME = "hello.md";

    FileReader fr;
    fr.setFilename(FILENAME);
    QCOMPARE(fr.m_filename, FILENAME);
    QCOMPARE(fr.filename(), FILENAME);
    QCOMPARE(fr.property("filename").toString(), FILENAME);
}

void tst_FileReader::isValid_with_invalid_folder()
{
    const auto invalidDir = makeInvalidDir();

    FileReader fr;
    fr.setFilename(invalidDir.filePath("killroy.md"));
    QCOMPARE(fr.isValid(), false);
}

void tst_FileReader::isValid_with_invalid_file()
{
    const auto invalidDir = makeInvalidDir();

    FileReader fr;
    fr.setFilename(invalidDir.filePath("killroy.md"));
    QCOMPARE(fr.isValid(), false);
}

void tst_FileReader::isValid()
{
    QTemporaryFile tempFile;
    tempFile.open();

    FileReader fr;
    fr.setFilename(tempFile.fileName());
    QCOMPARE(fr.isValid(), true);
}


void tst_FileReader::load_file()
{
    QFile file;
    makeTemporaryFile(file, "testfile.md");

    FileReader fr;

    QSignalSpy contentSpy(&fr, &FileReader::contentChanged);
    QSignalSpy filenameSpy(&fr, &FileReader::filenameChanged);

    fr.load(file.fileName());

    QCOMPARE(filenameSpy.count(), 1);
    QCOMPARE(filenameSpy.first()[0].toString(), fr.filename());
    QCOMPARE(contentSpy.count(), 1);
}

void tst_FileReader::reload_file()
{
    QFile file;
    makeTemporaryFile(file, "testfile.md");

    FileReader fr;

    QSignalSpy contentSpy(&fr, &FileReader::contentChanged);
    QSignalSpy filenameSpy(&fr, &FileReader::filenameChanged);

    fr.load(file.fileName());

    // modify file
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    file.write(QByteArray("-updated!"));
    file.close();

    fr.load(file.fileName());

    QCOMPARE(filenameSpy.count(), 1);
    QCOMPARE(filenameSpy.first()[0].toString(), fr.filename());
    QCOMPARE(fr.content(), defaultContent + "-updated!");
    QCOMPARE(contentSpy.count(), 2);
    QVERIFY(contentSpy.first().isEmpty());
    QVERIFY(contentSpy[1].isEmpty());
}

QTEST_MAIN(tst_FileReader)
#include "tst_FileReader.moc"
