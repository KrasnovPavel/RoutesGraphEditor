#include "linesmodel.h"

LinesModel::LinesModel(QObject *parent) : QAbstractListModel(parent)
{

}

int LinesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_lines.size();
}

QVariant LinesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case FirstNodeRole:
        return m_lines.at(index.row()).firstNode;
    case SecondNodeRole:
        return m_lines.at(index.row()).secondNode;
    case TypeRole:
        return m_lines.at(index.row()).type;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> LinesModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[FirstNodeRole] = "firstNode";
    roles[SecondNodeRole] = "secondNode";
    roles[TypeRole] = "type";
    return roles;
}

bool LinesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    switch (role) {
    case TypeRole:
        if (m_lines[index.row()].type != value.toInt()) {
            m_lines[index.row()].type = value.toInt();
            emit dataChanged(index, index, QVector<int>() << role);
            return true;
        }
        break;
    case SecondNodeRole:
        if (m_lines[index.row()].secondNode != value.toInt()
            && m_lines[index.row()].secondNode != m_lines[index.row()].firstNode) {
            m_lines[index.row()].secondNode = value.toInt();
            emit dataChanged(index, index, QVector<int>() << role);
            return true;
        }
        break;
    }

    return false;
}

Qt::ItemFlags LinesModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

void LinesModel::append(int firstNode, int secondNode, int type)
{
    for (int i = 0; i < m_lines.size(); i++)
    {
        if (m_lines.at(i).firstNode == secondNode && m_lines.at(i).secondNode == firstNode)
        {
            m_lines[i].type = 3;
            emit dataChanged(this->index(i), this->index(i), QVector<int>() << TypeRole);
            return;
        }
    }

    beginInsertRows(QModelIndex(), m_lines.size(), m_lines.size());
    m_lines.append({firstNode, secondNode, type});
    endInsertRows();
}

void LinesModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_lines.size());
    m_lines.clear();
    endRemoveRows();
}

void LinesModel::remove(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    m_lines.removeAt(index);
    endRemoveRows();
}

void LinesModel::nodeRemoved(int nodeIndex)
{
    for (int i = m_lines.size() - 1; i >= 0; i--) {
        if (m_lines.at(i).firstNode == nodeIndex || m_lines.at(i).secondNode == nodeIndex) {
            beginRemoveRows(QModelIndex(), i, i);
            m_lines.removeAt(i);
            endRemoveRows();
        }
    }
}
