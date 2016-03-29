#include "sample_model.hpp"

#include <cassert>

#include <QtCore/QDebug>

SampleModel::
SampleModel()
{
    for (auto i = 0u; i < 20; ++i) {
        SampleData item {
            QString::fromUtf8("Item No.%1").arg(i),
            QUrl { "http://www.adblock.org" },
            QUrl { "http://www.google.com" },
            QDateTime::currentDateTime(),
            i % 2 == 0
        };
        m_data.push_back(std::move(item));
    }
}

QVariant SampleModel::
data(const QModelIndex &index,
     const int role /*= Qt::DisplayRole*/) const
{
    if (role != Qt::DisplayRole) return {};

    const auto row = index.row();
    const auto column = index.column();

    const auto &item = m_data[static_cast<size_t>(row)];
    switch (column) {
    case 0:
        return item.title;
    case 1:
        return item.homePage;
    case 2:
        return item.listLocation;
    case 3:
        return item.updated;
    case 4:
        return item.updateStatus;
    default:
        assert(false);
        return {};
    }
}

Qt::ItemFlags SampleModel::
flags(const QModelIndex&) const
{
    return Qt::ItemIsSelectable |
           Qt::ItemIsEnabled;
}

int SampleModel::
rowCount(const QModelIndex&/* = QModelIndex {}*/) const
{
    return static_cast<int>(m_data.size());
}

int SampleModel::
columnCount(const QModelIndex&/* = QModelIndex {}*/) const
{
    return 5;
}

QModelIndex SampleModel::
index(const int row, const int column,
      const QModelIndex &parent/* = QModelIndex {}*/) const
{
    if (parent.isValid())        return {};
    if (column >= columnCount()) return {};
    if (row >= rowCount())       return {};

    return this->createIndex(row, column, nullptr);
}

QModelIndex SampleModel::
parent(const QModelIndex&) const
{
    return {};
}

bool SampleModel::
setData(const QModelIndex &index, const QVariant &value,
                                  const int role/* = Qt::EditRole*/)
{
    if (role != Qt::EditRole) return false;

    assert(index.isValid());
    assert(value.isValid());

    const auto row = index.row();
    assert(0 <= row && row < rowCount());

    auto &data = m_data[static_cast<size_t>(row)];

    const auto column = index.column();
    switch (column) {
    case 0:
        data.title = value.toString();
        break;
    case 1:
        data.homePage = value.toUrl();
        break;
    case 2:
        data.listLocation = value.toUrl();
        break;
    case 3:
        data.updated = value.toDateTime();
        break;
    case 4:
        data.updateStatus = value.toBool();
        break;
    default:
        assert(false);
    }

    Q_EMIT dataChanged(index, index);

    return true;
}

bool SampleModel::
insertRows(const int row, const int count,
           const QModelIndex &parent/* = QModelIndex {}*/)
{
    assert(0 <= row && row <= rowCount());
    assert(count > 0);

    this->beginInsertRows(parent, row, row + count - 1);

    const auto begin = m_data.begin() + row;
    const auto end = begin + count;

    for (auto it = begin; it != end; ++it) {
        m_data.emplace(it);
    }

    this->endInsertRows();

    return true;
}

bool SampleModel::
removeRows(const int row, const int count,
           const QModelIndex &parent/* = QModelIndex {}*/)
{
    assert(0 <= row && row < rowCount());
    assert(row + count <= rowCount());

    this->beginRemoveRows(parent, row, row + count - 1);

    const auto begin = m_data.begin() + row;
    const auto end = begin + count;
    m_data.erase(begin, end);

    this->endRemoveRows();

    return true;
}
