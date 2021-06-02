#ifndef GTTEXTDARKHIGHLIGHTER_H
#define GTTEXTDARKHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class GTTextDarkHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
private:
    QStringList m_lstKeywords;
protected:
    enum{normalState = -1, insideCStyleComment, insideCString};
    virtual void highlightBlock(const QString& str);
    QString getKeyword(int i, const QString& str) const;
public:
    GTTextDarkHighlighter(QTextDocument* parent = nullptr);
    ~GTTextDarkHighlighter();
};

#endif // GTTEXTDARKHIGHLIGHTER_H
