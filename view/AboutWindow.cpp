#include "AboutWindow.h"

#include <QFile>

AboutWindow::AboutWindow(QWidget *parent)
{
    setWindowTitle("About Program");
    setFixedSize(400, 400);
    setWindowFlag(Qt::Dialog);
    setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_DeleteOnClose);

    m_edit = new QTextEdit();
    m_edit->setReadOnly(true);
    m_edit->setAlignment(Qt::AlignLeft);
    m_edit->setTextInteractionFlags(Qt::TextBrowserInteraction);
    m_edit->setFont(QFont("Times New Roman", 10));
    m_edit->resize(400, 400);
    m_edit->setWordWrapMode(QTextOption::WordWrap);
    QFile file(":/resources/README.md");
    file.open(QFile::ReadOnly);
    m_edit->setMarkdown(file.readAll());
    m_layout = new QVBoxLayout();
    m_layout->addWidget(m_edit);
    setLayout(m_layout);
}