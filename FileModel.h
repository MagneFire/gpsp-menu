#ifndef FILEMODEL_H
#define FILEMODEL_H

#include "FileItem.h"
#include <QAbstractListModel>

class FileModel : public QAbstractListModel
{
    Q_OBJECT
    //Q_PROPERTY(QList<Animal> animals READ getListData NOTIFY animalModelChanged)

public:
    enum AnimalRoles {
    TitleRole = Qt::UserRole + 1,
    PathRole
    };

    FileModel(QObject *parent = 0);
    virtual ~FileModel() {};

    void addFile(const FileItem &file);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE void run(const QString size);
signals:
    void fileModelChanged();

protected:
    QHash<int, QByteArray> roleNames() const;
    private:
    QList<FileItem> m_files;
};
#endif