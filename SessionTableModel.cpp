#include "SessionTableModel.h"
#include <iomanip>
#include <sstream>
#include <QString>

SessionTableModel::SessionTableModel(QObject* parent)
    : QAbstractTableModel(parent), movie(nullptr) {}

void SessionTableModel::setMovie(Movie* m) {
    beginResetModel();
    movie = m;
    endResetModel();
}

int SessionTableModel::rowCount(const QModelIndex &) const {
    return movie ? movie->getSessionCount() : 0;
}

int SessionTableModel::columnCount(const QModelIndex &) const {
    return 1;
}

QVariant SessionTableModel::data(const QModelIndex &index, int role) const {
    if (!movie || !index.isValid() || role != Qt::DisplayRole) return {};

    std::vector<std::tm> sessions = movie->getSessionTimes();
    if (index.row() >= 0 && index.row() < static_cast<int>(sessions.size())) {
        std::ostringstream oss;
        oss << std::put_time(&sessions[index.row()], "%Y-%m-%d %H:%M");
        return QString::fromStdString(oss.str());
    }
    return {};
}

QVariant SessionTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return "Date & Time";
    }
    return {};
}
