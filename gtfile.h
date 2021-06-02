#ifndef GTFILE_H
#define GTFILE_H

#include <QFile>
#include <QTextEdit>
#include <QObject>
#include <QSyntaxHighlighter>

#include "gttextlighthighlighter.h"
#include "gttextdarkhighlighter.h"

class GTFile : public QWidget
{
    Q_OBJECT
private:
    QString m_sFilePath;
    QString m_sFileName;
    QFile* m_pFile;
    QTextEdit* m_pTextEdit;
    bool m_bIsSaved;

    QSyntaxHighlighter* m_pHighlighter;
    int m_nTheme;

    enum Theme{LightTheme, DarkTheme};
public:
    GTFile(const QString path, int theme, QWidget* parent = nullptr);
    ~GTFile();

    //Геттеры и сеттеры
    QString getFilePath();
    void setFilePath(QString path);

    QString getFileName();
    void setFileName(QString name);

    QTextEdit*  getTextEdit();
    void setTextEdit(QTextEdit* textEdit);;

    QFile* getFile();
    void setFile(QFile* file);

    //Меттоды
    void findFileName();
    bool isSaved();
    void saveFile();
    void deleteConnectedFile();

    //методы для смены темы
    void setLightTheme();
    void setDarkTheme();
    void setLightHighlighter();
    void setDarkHighlighter();

public slots:
    void fileIsNotSavedSlot();
signals:
    void fileIsNotSavedSignal();
    void connectedFileDeleted();
};

#endif // GTFILE_H
