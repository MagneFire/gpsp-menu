#ifndef FILEITEM_H
#define FILEITEM_H

#include <QObject>

class FileItem
{
public:
    FileItem(const QString &title, const QString &path);
    virtual ~FileItem() {};

    QString title() const;
    QString path() const;

private:
    const QString m_title;
    const QString m_path;
};

#endif