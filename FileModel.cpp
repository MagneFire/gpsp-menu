#include "FileModel.h"
#include <QDebug>

FileModel::FileModel(QObject *parent)
: QAbstractListModel(parent)
{

}

void FileModel::addFile(const FileItem &file)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_files << file;
    endInsertRows();
}

int FileModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_files.count();
}

QVariant FileModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_files.count())
        return QVariant();

    const FileItem &file = m_files[index.row()];
    if (role == TitleRole)
        return file.title();
    else if (role == PathRole)
        return file.path();
    else if (role == IdRole)
        return index.row();
    return QVariant();
}

QHash<int, QByteArray> FileModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[PathRole] = "path";
    roles[IdRole] = "index";
    return roles;
}