#include "addquestiondialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>

AddQuestionDialog::AddQuestionDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Add Question");
    resize(500, 400);

    m_txtQuestion = new QTextEdit(this);
    m_leA = new QLineEdit(this);
    m_leB = new QLineEdit(this);
    m_leC = new QLineEdit(this);
    m_leD = new QLineEdit(this);
    m_cbCorrect = new QComboBox(this);
    m_cbCorrect->addItems({"A","B","C","D"});
    m_cbLevel = new QComboBox(this);
    m_cbLevel->addItems({"easy","medium","hard"});

    m_btnSave = new QPushButton("Save", this);
    m_btnCancel = new QPushButton("Cancel", this);

    QVBoxLayout *main = new QVBoxLayout(this);
    main->addWidget(new QLabel("Question:"));
    main->addWidget(m_txtQuestion);
    main->addWidget(new QLabel("Answer A:"));
    main->addWidget(m_leA);
    main->addWidget(new QLabel("Answer B:"));
    main->addWidget(m_leB);
    main->addWidget(new QLabel("Answer C:"));
    main->addWidget(m_leC);
    main->addWidget(new QLabel("Answer D:"));
    main->addWidget(m_leD);

    QHBoxLayout *h = new QHBoxLayout();
    h->addWidget(new QLabel("Correct:"));
    h->addWidget(m_cbCorrect);
    h->addWidget(new QLabel("Level:"));
    h->addWidget(m_cbLevel);
    main->addLayout(h);

    QHBoxLayout *hbtn = new QHBoxLayout();
    hbtn->addStretch();
    hbtn->addWidget(m_btnSave);
    hbtn->addWidget(m_btnCancel);
    main->addLayout(hbtn);

    connect(m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_btnSave, &QPushButton::clicked, this, &QDialog::accept);
}

Question AddQuestionDialog::question() const {
    Question q;
    q.question = m_txtQuestion->toPlainText();
    q.A = m_leA->text();
    q.B = m_leB->text();
    q.C = m_leC->text();
    q.D = m_leD->text();
    q.answer = m_cbCorrect->currentText();
    q.level = m_cbLevel->currentText();
    return q;
}
