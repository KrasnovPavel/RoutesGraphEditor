#ifndef DOTSMODEL_H
#define DOTSMODEL_H

#include <QAbstractListModel>
#include <QVector2D>

class DotsModel : public QAbstractListModel
{
    Q_OBJECT

    enum Roles {
        NodeIDRole = Qt::UserRole + 1,
        XRole ,
        YRole,
        TypeRole
    };

public:
    explicit DotsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    struct Dot {
        int nodeID;
        QVector2D position;
        int type;
    };

public slots:
    void append(int nodeID, int x, int y);
    void clear();
    void remove(int index);
    void setType(int nodeID, int type);
    int getNewID();
    int indexByID(int nodeID);
    const QList<Dot> dots() const { return m_dots; }

protected:
    QList<Dot> m_dots;
    int m_maxID = 0;
};

#endif // DOTSMODEL_H
