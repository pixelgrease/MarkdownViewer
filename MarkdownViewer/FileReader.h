#pragma once

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

