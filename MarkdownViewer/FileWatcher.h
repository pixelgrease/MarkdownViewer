#pragma once

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

