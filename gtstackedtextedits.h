#ifndef GTSTACKEDTEXTEDITS_H
#define GTSTACKEDTEXTEDITS_H
#include "gtfile.h"

#include <QStackedWidget>

class GTStackedTextEdits : public QStackedWidget
{
    Q_OBJECT
private:
    QVector<GTFile*> m_vAllFilesOfAddedTextEdits;

public:
    GTStackedTextEdits(QWidget* parent = nullptr);
    ~GTStackedTextEdits();

    //методы
    void removeCurrentTextEdit();

    //методы для смены темы
    void setLightTheme();
    void setDarkTheme();

public slots:
    void addTextEditSlot(GTFile* fileOfTextEditToAdd);

    void openProjectSlot(const QString& projectName, QVector<GTFile*>* allProjectFiles);
    void newFileSlot(GTFile* newFile);
    void closeProjectSlot();
    void setCurrentTextEditSlot(const QString& fileNameOfTextEdit);
};

#endif // GTSTACKEDTEXTEDITS_H
