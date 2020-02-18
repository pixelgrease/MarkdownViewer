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
