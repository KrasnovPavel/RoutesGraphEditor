#ifndef XMLHANDLER_H
#define XMLHANDLER_H

#include <QObject>
#include <QXmlStreamReader>

#include "dotsmodel.h"
#include "linesmodel.h"

class XmlHandler : public QObject
{
    Q_OBJECT
public:
    explicit XmlHandler(DotsModel *dotsModel, LinesModel *linesModel, QObject *parent = nullptr);

public slots:
    void openXML(QString fileName);
    void saveXML();

private:
    void parceNodes(QXmlStreamReader &xml);
    void parceLinks(QXmlStreamReader &xml);
    void parceObjects(QXmlStreamReader &xml);

    DotsModel *m_dotsModel;
    LinesModel *m_linesModel;
    QString m_fileName;
};

#endif // XMLHANDLER_H
