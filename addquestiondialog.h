#ifndef ADDQUESTIONDIALOG_H
#define ADDQUESTIONDIALOG_H

#include <QDialog>
#include "models/Question.h"

class QLineEdit;
class QTextEdit;
class QComboBox;
class QPushButton;

class AddQuestionDialog : public QDialog {
    Q_OBJECT
public:
    explicit AddQuestionDialog(QWidget *parent = nullptr);
    Question question() const;

private:
    QTextEdit *m_txtQuestion;
    QLineEdit *m_leA, *m_leB, *m_leC, *m_leD;
    QComboBox *m_cbCorrect, *m_cbLevel;
    QPushButton *m_btnSave, *m_btnCancel;
};;

#endif // ADDQUESTIONDIALOG_H
