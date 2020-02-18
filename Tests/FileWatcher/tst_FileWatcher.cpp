#include "FileWatcher.h"
#include <QSignalSpy>
#include <QtTest>

// add necessary includes here

class tst_FileWatcher : public QObject
{
    Q_OBJECT

public:
    tst_FileWatcher();
    ~tst_FileWatcher();

private slots:
    void initialization();
    void filename_member();
    void filename_property();
    void isValid_with_invalid_folder();
    void isValid_with_invalid_file();
    void isValid();
    void signals_on_modification();
    void no_signal_on_modifying_other_file();
    void watch_a_different_file();
    void setting_file_emits_signal();
    void setting_file_sets_isValid();
//    void three_changes_notified_after_100_ms();

private:
    QDir makeInvalidDir();
    void makeTemporaryFile(QFile& file, const QString &filename);

    QTemporaryDir m_tempDir;
    const QString defaultContent;
};

tst_FileWatcher::tst_FileWatcher() : defaultContent("hello, world!")
{
    const QString tempDirPattern = QDir::cleanPath(QDir::tempPath() +
                                                   QDir::separator() +
                                                   "tst_FileWatcher");
    QTemporaryDir tempDir(tempDirPattern);
}

tst_FileWatcher::~tst_FileWatcher()
{
}

QDir tst_FileWatcher::makeInvalidDir()
{
    QTemporaryDir tempDir;
    const QDir dir(tempDir.path());
    tempDir.remove();
    return dir;
}

void tst_FileWatcher::makeTemporaryFile(QFile& file, const QString &filename)
{
    file.setFileName(m_tempDir.path() + QDir::separator() + filename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.write(QByteArray(qPrintable(defaultContent)));
    file.close();
}

void tst_FileWatcher::initialization()
{
    FileWatcher fw;
    QVERIFY(fw.m_filename.isEmpty());
}

void tst_FileWatcher::filename_member()
{
    const QString FILENAME = "hello.md";

    FileWatcher fw;
    fw.setFilename(FILENAME);
    QCOMPARE(fw.m_filename, FILENAME);
    QCOMPARE(fw.filename(), FILENAME);
    QCOMPARE(fw.property("filename"), FILENAME);
}

void tst_FileWatcher::filename_property()
{
    const QString FILENAME = "hello.md";

    FileWatcher fw;
    fw.setProperty("filename", FILENAME);
    QCOMPARE(fw.m_filename, FILENAME);
    QCOMPARE(fw.filename(), FILENAME);
    QCOMPARE(fw.property("filename"), FILENAME);
}

void tst_FileWatcher::isValid_with_invalid_folder()
{
    const auto invalidDir = makeInvalidDir();

    FileWatcher fw;
    fw.setFilename(invalidDir.filePath("killroy.md"));
    QCOMPARE(fw.isValid(), false);
}

void tst_FileWatcher::isValid_with_invalid_file()
{
    const auto invalidDir = makeInvalidDir();

    FileWatcher fw;
    fw.setFilename(invalidDir.filePath("killroy.md"));
    QCOMPARE(fw.isValid(), false);
}

void tst_FileWatcher::isValid()
{
    QTemporaryFile tempFile;
    tempFile.open();

    FileWatcher fw;
    fw.setFilename(tempFile.fileName());
    QCOMPARE(fw.isValid(), true);
}

void tst_FileWatcher::signals_on_modification()
{
    QFile file;
    makeTemporaryFile(file, "testfile.md");

    FileWatcher fw;
    fw.setFilename(file.fileName());

    QTest::qWait(100);

    QSignalSpy changeSpy(&fw, &FileWatcher::changed);

    // modify file
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    file.write(QByteArray("update 1"));
    file.close();

    QTRY_COMPARE(changeSpy.count(), 1);
    QCOMPARE(changeSpy.count(), 1);
}

void tst_FileWatcher::no_signal_on_modifying_other_file()
{
    QFile file, otherFile;
    makeTemporaryFile(file, "testfile.md");
    makeTemporaryFile(otherFile, "leper_lover_by_death_tongue.md");

    FileWatcher fw;
    fw.setFilename(file.fileName());

    QTest::qWait(100);

    QSignalSpy spy(&fw, &FileWatcher::changed);

    // modify other file
    otherFile.open(QIODevice::WriteOnly | QIODevice::Append);
    otherFile.write(QByteArray("updated"));
    otherFile.close();

    QTRY_COMPARE(spy.count(), 0);
    QCOMPARE(spy.count(), 0);
}

void tst_FileWatcher::watch_a_different_file()
{
    QFile file, otherFile;
    makeTemporaryFile(file, "testfile.md");
    makeTemporaryFile(otherFile, "leper_lover_by_death_tongue.md");

    FileWatcher fw;
    fw.setFilename(file.fileName());

    QSignalSpy filenameSpy(&fw, &FileWatcher::filenameChanged);

    fw.setFilename(otherFile.fileName());

    QTest::qWait(100);

    QCOMPARE(filenameSpy.count(), 1);
    QCOMPARE(filenameSpy.first()[0].toString(), fw.filename());

    // modify other file
    otherFile.open(QIODevice::WriteOnly | QIODevice::Append);
    otherFile.write(QByteArray("updated"));
    otherFile.close();
}

void tst_FileWatcher::setting_file_emits_signal()
{
    QFile file;
    makeTemporaryFile(file, "testfile.md");

    FileWatcher fw;

    QSignalSpy changeSpy(&fw, &FileWatcher::changed);
    QSignalSpy filenameSpy(&fw, &FileWatcher::filenameChanged);

    fw.setFilename(file.fileName());

    QTest::qWait(100);

    QTRY_COMPARE(changeSpy.count(), 1);
    QCOMPARE(changeSpy.count(), 1);

    QTRY_COMPARE(filenameSpy.count(), 1);
    QCOMPARE(filenameSpy.count(), 1);
    QCOMPARE(filenameSpy.first()[0].toString(), fw.filename());
}

void tst_FileWatcher::setting_file_sets_isValid()
{
    QFile file;
    makeTemporaryFile(file, "testfile.md");

    FileWatcher fw;

    QSignalSpy spy(&fw, &FileWatcher::isValidChanged);

    fw.setFilename(file.fileName());

    QTest::qWait(100);

    QTRY_COMPARE(spy.count(), 1);
    QCOMPARE(spy.count(), 1);
}


QTEST_MAIN(tst_FileWatcher)
#include "tst_FileWatcher.moc"
