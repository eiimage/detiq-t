#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QMessageBox>

/**
 * @brief The AboutDialog class represents the About window of the
 */
class AboutDialog : public QMessageBox
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = 0);

signals:

public slots:

};

#endif // ABOUTDIALOG_H
