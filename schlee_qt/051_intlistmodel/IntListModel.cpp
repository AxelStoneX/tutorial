#include "IntListModel.h"

IntListModel::IntListModel(const QList<int> &list, QObject *pobj)
    :QAbstractListModel(pobj)
    ,m_list(list)
{
}

QVariant IntListModel::data(const QModelIndex &index, int nRole) const
{
    if (!index.isValid())
        return QVariant();                                       // Возвращаем пустой объект QVariant
    return (nRole == Qt::DisplayRole || nRole == Qt::EditRole)
            ? m_list.at(index.row())
            : QVariant();                                        // Возвращаем пустой объект QVariant
}

bool IntListModel::setdata(const QModelIndex &index,
                           const QVariant    &value,
                           int               nRole   )
{
    if (index.isValid() && nRole == Qt::EditRole)
    {
        m_list.replace(index.row(), value.value<int>());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

int IntListModel::rowCount(const QModelIndex &parent) const
{
    return m_list.size();
}

QVariant IntListModel::headerData(int             nSection,
                                  Qt::Orientation orientation,
                                  int             nRole        ) const
{
    if (nRole != Qt::DisplayRole)
        return QVariant();
    return (orientation == Qt::Horizontal) ? QString("Number")
                                           : QString::number(nSection);
}

Qt::ItemFlags IntListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractListModel::flags(index);
    return index.isValid() ? (flags | Qt::ItemIsEditable)
                           : flags;
}
