#ifndef LINESMODEL_H
#define LINESMODEL_H

#include <QAbstractListModel>

class LinesModel : public QAbstractListModel
{
    Q_OBJECT
    enum Roles {
        FirstNodeRole = Qt::UserRole + 1,
        SecondNodeRole,
        TypeRole
    };

public:
    explicit LinesModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    struct Line {
        int firstNode;
        int secondNode;
        int type;
    };

public slots:
    void append(int firstNode, int secondNode = -1, int type = 3);
    void clear();
    void remove(int index);
    void nodeRemoved(int nodeIndex);
    const QList<Line> lines() const { return m_lines; }

protected:
    QList<Line> m_lines;
};

#endif // LINESMODEL_H
