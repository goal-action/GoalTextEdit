#ifndef GTTEXTLIGHTHIGHLIGHTER_H
#define GTTEXTLIGHTHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class GTTextLightHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
private:
    QStringList m_lstKeywords;
protected:
    enum{normalState = -1, insideCStyleComment, insideCString};
    virtual void highlightBlock(const QString& str);
    QString getKeyword(int i, const QString& str) const;
public:
    GTTextLightHighlighter(QTextDocument* parent = nullptr);
    ~GTTextLightHighlighter();
};

#endif // GTTEXTLIGHTHIGHLIGHTER_H
