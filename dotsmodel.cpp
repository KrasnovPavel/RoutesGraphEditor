#include "dotsmodel.h"

DotsModel::DotsModel(QObject *parent) : QAbstractListModel(parent)
{

}

int DotsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_dots.size();
}

QVariant DotsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case NodeIDRole:
        return m_dots.at(index.row()).nodeID;
    case XRole:
        return m_dots.at(index.row()).position.x();
    case YRole:
        return m_dots.at(index.row()).position.y();
    case TypeRole:
        return m_dots.at(index.row()).type;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> DotsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[NodeIDRole] = "nodeID";
    roles[XRole] = "x";
    roles[YRole] = "y";
    roles[TypeRole] = "type";
    return roles;
}

bool DotsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    if (role == TypeRole && m_dots[index.row()].type != value.toInt()) {
        m_dots[index.row()].type = value.toInt();
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}

Qt::ItemFlags DotsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

void DotsModel::append(int nodeID, int x, int y)
{
    beginInsertRows(QModelIndex(), m_dots.size(), m_dots.size());
    m_dots.append({nodeID, QVector2D(x, y), 0});
    endInsertRows();

    if (nodeID > m_maxID) m_maxID = nodeID;
}

void DotsModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_dots.size());
    m_dots.clear();
    endRemoveRows();
}

void DotsModel::remove(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    m_dots.removeAt(index);
    endRemoveRows();
}

void DotsModel::setType(int nodeID, int type)
{
    for (int i = 0; i < m_dots.size(); ++i)
    {
        if (m_dots[i].nodeID == nodeID && m_dots[i].type != type)
        {
            m_dots[i].type = type;
            emit dataChanged(this->index(i), this->index(i), QVector<int>() << TypeRole);
            return;
        }
    }
}

int DotsModel::getNewID()
{
    return m_maxID + 1;
}

int DotsModel::indexByID(int nodeID)
{
    for (int i = 0; i < m_dots.size(); i++)
    {
        if (m_dots.at(i).nodeID == nodeID)
        {
            return i;
        }
    }
}
