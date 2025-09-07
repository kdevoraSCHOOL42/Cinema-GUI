#ifndef DIALOGADDMOVIE_H
#define DIALOGADDMOVIE_H

#include <QDialog>

namespace Ui {
class DialogAddMovie;
}

class DialogAddMovie : public QDialog {
    Q_OBJECT

public:
    explicit DialogAddMovie(QWidget *parent = nullptr);
    ~DialogAddMovie();

    QString getTitle() const;
    int getDuration() const;

private:
    Ui::DialogAddMovie *ui;
};

#endif // DIALOGADDMOVIE_H
