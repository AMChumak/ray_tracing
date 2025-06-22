#pragma once
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

class AboutWindow : public QWidget
{
    Q_OBJECT
public:
    AboutWindow(QWidget *parent = nullptr);

private:
    QTextEdit *m_edit;
    QVBoxLayout *m_layout;
};