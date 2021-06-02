#ifndef GTSTATUSBAR_H
#define GTSTATUSBAR_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QFile>

class GTStatusBar : public QWidget
{
    Q_OBJECT
private:
    QLabel* m_pStatus;
public:
    explicit GTStatusBar(QWidget *parent = nullptr);
    ~GTStatusBar();

    //Методы
    void setStatus(QString newStatus);

    //методы для смены темы
    void setLightTheme();
    void setDarkTheme();

public slots:

signals:

};

#endif // GTSTATUSBAR_H
