#ifndef MOVIETABLEMODEL_H
#define MOVIETABLEMODEL_H

#include <QAbstractTableModel>
#include "CinemaSchedule.h"

class MovieTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    MovieTableModel(CinemaSchedule* schedule, QObject* parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    CinemaSchedule* schedule;
};

#endif // MOVIETABLEMODEL_H
