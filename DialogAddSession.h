#ifndef DIALOGADDSESSION_H
#define DIALOGADDSESSION_H

#include <QDialog>
#include <ctime>

namespace Ui {
class DialogAddSession;
}

class DialogAddSession : public QDialog {
    Q_OBJECT

public:
    explicit DialogAddSession(QWidget *parent = nullptr);
    ~DialogAddSession();

    std::tm getDateTime() const;

private:
    Ui::DialogAddSession *ui;
};

#endif // DIALOGADDSESSION_H
