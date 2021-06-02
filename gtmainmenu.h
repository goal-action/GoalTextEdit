#ifndef GTMAINMENU_H
#define GTMAINMENU_H

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFile>

class GTMainMenu : public QMenuBar
{
    Q_OBJECT
private:
    QVector<QAction*>* m_pvAvailableActionsWithProjectAndFiles;
    QVector<QAction*>* m_pvAvailableActionsWithProject;
    QVector<QMenu*>* m_pvAllMenus;
public:
    GTMainMenu(QWidget* parent = nullptr);
    ~GTMainMenu();

    //методы для открытия и закрытия пунктов  меню
    void setProjectExistActionsAvailable();
    void setProjectAndFileExistActionsAvailable();
    void setProjectExistActionsUnAvailable();
    void setProjectAndFileExistActionsUnAvailable();

    //методы для смены темы
    void setLightTheme();
    void setDarkTheme();

public slots:

signals:
    void newProjectPressedSignal();
    void openProjectPressedSignal();
    void closeProjectPressedSignal();
    void newFilePressedSignal();
    void deleteFilePressedSignal();
    void saveProjectPressedSignal();
    void savefilePressedSignal();
    void exitPressedSignal();
    void runNowPressedSignal();
    void aboutPressedSignal();
    void leftSideBarPressedSignal();
    void rightSideBarPressedSignal();
    void lightThemePressedSignal();
    void darkThemePressedSignal();
};

#endif // GTMAINMENU_H
