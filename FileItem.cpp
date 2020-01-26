#include "FileItem.h"

FileItem::FileItem(const QString &title, const QString &path)
    : m_title(title)
    , m_path(path)
{

}

QString FileItem::title() const
{
    return m_title;
}

QString FileItem::path() const
{
    return m_path;
}