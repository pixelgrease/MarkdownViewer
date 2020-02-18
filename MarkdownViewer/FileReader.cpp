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

    if (m_isValid) {
        QFile f(filename);

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
