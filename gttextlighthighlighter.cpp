#include "gttextlighthighlighter.h"

GTTextLightHighlighter::GTTextLightHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    m_lstKeywords << "while" << "bool" << "int" << "void" << "double"
    << "float" << "char" << "delete" << "class" << "const"
    << "virtual" << "mutable" << "this" << "struct" << "union"
    << "throw" << "for" << "if" << "else" << "false"
    << "namespace" << "switch" << "case" << "public" << "private"
    << "protected" << "new" << "return" << "using" << "true";
}

void GTTextLightHighlighter::highlightBlock(const QString& str){
    int nState = previousBlockState();
    int nStart = 0;
    for(int i = 0; i < str.size(); i++){
        if(nState == insideCStyleComment){
            if(str.mid(i, 2) == "*/"){
                nState = normalState;
                setFormat(nStart, i - nStart + 2, "#666966");
                i++;
            }
        }
        else if(nState == insideCString){
            if(str.mid(i, 1) == "\"" || str.mid(i, 1) == "\'"){
                if(str.mid(i - 1, 2) != "\\\"" || str.mid(i - 1, 2) != "\\\'"){
                    nState = normalState;
                    setFormat(nStart, i - nStart + 1, "#0b9500");
                }
            }
        }
        else{
            if(str.mid(i, 2) == "//"){
                setFormat(i, str.size() - i, "#666966");
                break;
            }else if(str.mid(i, 1) == "#"){
                setFormat(i, str.size() - i, "#9d1600");
                break;
            }else if(str.at(i).isNumber()){
                setFormat(i, 1, "#010a81");
            }else if(str.mid(i, 2) == "/*"){
                nState = insideCStyleComment;
                nStart = i;
            }else if(str.mid(i, 1) == "\"" || str.mid(i, 1) == "\'"){
                nState = insideCString;
                nStart = i;
            }
            else{
                QString strKeyWord = getKeyword(i, str);
                if(i != 0){
                    if(!strKeyWord.isEmpty() && !str.at(i - 1).isLetterOrNumber()){
                        setFormat(i, strKeyWord.length(), "#096272");
                        i+=strKeyWord.length() - 1;
                    }
                }else if(!strKeyWord.isEmpty()){
                     setFormat(i, strKeyWord.length(), "#096272");
                     i+=strKeyWord.length() - 1;
                }

            }
        }
    }
    if(nState == insideCString){
        setFormat(nStart, str.length() - nStart, "#0b9500");
    }
    if(nState == insideCStyleComment){
        setFormat(nStart, str.length() - nStart, "#666966");
    }
    setCurrentBlockState(nState);
}

QString GTTextLightHighlighter::getKeyword(int nPos, const QString& str) const{
    QString strTemp("");
    foreach(QString keyWord, m_lstKeywords){
        if(str.mid(nPos, keyWord.length()) == keyWord){
            strTemp = keyWord;
            return strTemp;
        }
    }

    return strTemp;
}

GTTextLightHighlighter::~GTTextLightHighlighter(){

}
