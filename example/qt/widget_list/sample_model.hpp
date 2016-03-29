#ifndef SAMPLE_MODEL_HPP
#define SAMPLE_MODEL_HPP

#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <QtCore/QUrl>

struct SampleData
{
    QString   title;
    QUrl      homePage;
    QUrl      listLocation;
    QDateTime updated;
    bool      updateStatus;
};

#include <QtCore/QAbstractItemModel>

class SampleModel : public QAbstractItemModel
{
public:
    SampleModel();

    // accessor
    // @override QAbstractItemModel
    QVariant data(const QModelIndex&,
                  const int role = Qt::DisplayRole) const override;
    // query
    // @overide QAbstractItemModel
    Qt::ItemFlags flags(const QModelIndex&) const override;
    int rowCount(const QModelIndex &parent = QModelIndex {}) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex index(const int row, const int column,
                      const QModelIndex &parent = QModelIndex {}) const override;
    QModelIndex parent(const QModelIndex&) const override;

    // modifier
    // @overide QAbstractItemModel
    bool setData(const QModelIndex&, const QVariant&,
                                     int role = Qt::EditRole) override;
    bool insertRows(const int row, const int count,
                    const QModelIndex &parent = QModelIndex {}) override;
    bool removeRows(const int row, const int count,
                    const QModelIndex &parent = QModelIndex {}) override;

private:
    std::vector<SampleData> m_data;
};

#endif // SAMPLE_MODEL_HPP
