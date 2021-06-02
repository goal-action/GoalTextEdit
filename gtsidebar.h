#ifndef GTSIDEBAR_H
#define GTSIDEBAR_H

#include "gtfile.h"

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QListWidgetItem>


class GTSideBar : public QWidget
{
    Q_OBJECT
private:
    QListWidget* m_pListNameOfFileWidget;
    QLabel* m_plblProjectName;
public:
    explicit GTSideBar(QWidget *parent = nullptr);
    ~GTSideBar();

    //Методы
    void setProjectName(const QString& newName);
    void deleteCurrentFile();

    //Методы для смены темы
    void setLightTheme();
    void setDarkTheme();

public slots:
    void changedCurrentItemSlot();
    void addItemSlot(const QString& itemName);
    void openProjectSlot(const QString& projectName, QVector<GTFile*>* allProjectFiles);
    void closeProjectSlot();
    void newFileSlot(GTFile*);

signals:
    void changedCurrentItemSignal(const QString& itemName);
};

#endif // GTSIDEBAR_H
