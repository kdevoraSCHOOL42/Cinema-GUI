#ifndef SESSIONTABLEMODEL_H
#define SESSIONTABLEMODEL_H

#include <QAbstractTableModel>
#include "Movie.h"

class SessionTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    SessionTableModel(QObject* parent = nullptr);
    void setMovie(Movie* movie);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    Movie* movie;
};

#endif // SESSIONTABLEMODEL_H
