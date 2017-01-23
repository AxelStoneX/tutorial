#ifndef INTLISTMODEL_H
#define INTLISTMODEL_H

#include <QAbstractListModel>

// ===========================================================================
class IntListModel : public QAbstractListModel
{
    Q_OBJECT
private:
    QList<int> m_list;
public:
    IntListModel(const QList<int>& list, QObject* pobj = 0);

    QVariant data(const QModelIndex& index, int nRole) const;

    bool setdata(const QModelIndex& index,
                 const QVariant&    value,
                 int                nRole);

    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    QVariant headerData(int             nSection,
                        Qt::Orientation orientation,
                        int nRole = Qt::DisplayRole  ) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // INTLISTMODEL_H
