#include "MovieTableModel.h"

MovieTableModel::MovieTableModel(CinemaSchedule* schedule, QObject* parent)
    : QAbstractTableModel(parent), schedule(schedule) {}

int MovieTableModel::rowCount(const QModelIndex &) const {
    return schedule->getMovieCount();
}

int MovieTableModel::columnCount(const QModelIndex &) const {
    return 4;
}

QVariant MovieTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) return {};

    Movie* movie = schedule->getMovieAt(index.row());
    if (!movie) return {};

    switch (index.column()) {
        case 0: return QString::fromStdString(movie->getTitle());
        case 1: return movie->getDurationMinutes();
        case 2: return movie->getSessionCount();
        case 3: return movie->getTotalDuration();
    }
    return {};
}

QVariant MovieTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) return {};

    switch (section) {
        case 0: return "Title";
        case 1: return "Duration (min)";
        case 2: return "Sessions";
        case 3: return "Total Time";
    }
    return {};
}
