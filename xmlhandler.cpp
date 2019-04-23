#include "xmlhandler.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QtDebug>

XmlHandler::XmlHandler(DotsModel *dotsModel, LinesModel *linesModel, QObject *parent)
    : QObject(parent), m_dotsModel(dotsModel), m_linesModel(linesModel)
{

}

void XmlHandler::openXML(QString fileName)
{
    m_fileName = fileName.remove(0, 7);
    QFile* file = new QFile(m_fileName);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can't open XML file: " << m_fileName << " " << file->errorString();
        return;
    }
    QXmlStreamReader xml(file);

    while (!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement)
        {
            if (xml.name() == "nodes")
            {
                parceNodes(xml);
            }
            else if (xml.name() == "links")
            {
                parceLinks(xml);
            }
            else if (xml.name() == "objects")
            {
                parceObjects(xml);
            }
        }
    }

    file->close();
    delete file;
}

void XmlHandler::saveXML()
{
    QFile* file = new QFile(m_fileName);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can't open XML file: " << m_fileName << " " << file->errorString();
        return;
    }

    QString data = QString(file->readAll());
    file->close();

    int nodesStart = data.indexOf("<nodes>") + 7;
    int nodesEnd = data.indexOf("</nodes>");

    data.remove(nodesStart, nodesEnd - nodesStart);
    for (int i = 0; i < m_dotsModel->dots().size(); ++i)
    {
        DotsModel::Dot dot = m_dotsModel->dots().at(i);
        data.insert(nodesStart, QString("<node id=\"%1\" x=\"%2\" y=\"%3\"/>\n").arg(dot.nodeID)
                                                                                .arg(dot.position.x())
                                                                                .arg(dot.position.y()));
    }

    int linksStart = data.indexOf("<links>") + 7;
    int linksEnd = data.indexOf("</links>");

    data.remove(linksStart, linksEnd - linksStart);
    for (int i = 0; i < m_linesModel->lines().size(); ++i)
    {
        LinesModel::Line line = m_linesModel->lines().at(i);
        switch (line.type) {
        case 1:
            data.insert(linksStart, QString("<link node1=\"%1\" node2=\"%2\"/>\n").arg(line.secondNode).arg(line.firstNode));
            break;
        case 2:
            data.insert(linksStart, QString("<link node1=\"%1\" node2=\"%2\"/>\n").arg(line.firstNode).arg(line.secondNode));
            break;
        case 3:
            data.insert(linksStart, QString("<link node1=\"%1\" node2=\"%2\"/>\n").arg(line.secondNode).arg(line.firstNode));
            data.insert(linksStart, QString("<link node1=\"%1\" node2=\"%2\"/>\n").arg(line.firstNode).arg(line.secondNode));
            break;
        }
    }

    int objectsStart = data.indexOf("<objects>") + 9;
    int objectsEnd = data.indexOf("</objects>");

    data.remove(objectsStart, objectsEnd - objectsStart);
    for (int i = 0; i < m_dotsModel->dots().size(); ++i)
    {
        DotsModel::Dot dot = m_dotsModel->dots().at(i);
        switch (dot.type) {
        case 1:
            data.insert(objectsStart, QString("<container id=\"%1\" node=\"%2\"/>\n").arg(i).arg(dot.nodeID));
            break;
        case 2:
            data.insert(objectsStart, QString("<station id=\"%1\" node=\"%2\"/>\n").arg(i).arg(dot.nodeID));
            break;
        case 3:
            data.insert(objectsStart, QString("<dump id=\"%1\" node=\"%2\"/>\n").arg(i).arg(dot.nodeID));
            break;
        }
    }

    if (!file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Can't open XML file: " << m_fileName << " " << file->errorString();
        return;
    }

    file->write(data.toUtf8());
    file->close();
}

void XmlHandler::parceNodes(QXmlStreamReader &xml)
{
    if (xml.tokenType() != QXmlStreamReader::StartElement && xml.name() == "nodes")
    {
        return;
    }

    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "nodes"))
    {
        if (xml.name() == "node")
        {
            QXmlStreamAttributes attributes = xml.attributes();
            if (attributes.hasAttribute("id") && attributes.hasAttribute("x") && attributes.hasAttribute("y"))
            {
                m_dotsModel->append(attributes.value("id").toInt(),
                                    attributes.value("x").toInt(),
                                    attributes.value("y").toInt());
            }
        }
        xml.readNext();
    }
}

void XmlHandler::parceLinks(QXmlStreamReader &xml)
{
    if (xml.tokenType() != QXmlStreamReader::StartElement && xml.name() == "links")
    {
        return;
    }

    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "links"))
    {
        if (xml.name() == "link")
        {
            QXmlStreamAttributes attributes = xml.attributes();
            if (attributes.hasAttribute("node1") && attributes.hasAttribute("node2"))
            {
                m_linesModel->append(attributes.value("node1").toInt(),
                                     attributes.value("node2").toInt(),
                                     2);
            }
        }
        xml.readNext();
    }
}

void XmlHandler::parceObjects(QXmlStreamReader &xml)
{
    if (xml.tokenType() != QXmlStreamReader::StartElement && xml.name() == "objects")
    {
        return;
    }

    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "objects"))
    {
        if (xml.tokenType() == QXmlStreamReader::StartElement && xml.name() == "container")
        {
            QXmlStreamAttributes attributes = xml.attributes();
            if (attributes.hasAttribute("node"))
            {
                m_dotsModel->setType(attributes.value("node").toInt(), 1);
            }
        }
        else if (xml.tokenType() == QXmlStreamReader::StartElement && xml.name() == "station")
        {
            QXmlStreamAttributes attributes = xml.attributes();
            if (attributes.hasAttribute("node"))
            {
                m_dotsModel->setType(attributes.value("node").toInt(), 2);
            }
        }
        else if (xml.tokenType() == QXmlStreamReader::StartElement && xml.name() == "dump")
        {
            QXmlStreamAttributes attributes = xml.attributes();
            if (attributes.hasAttribute("node"))
            {
                m_dotsModel->setType(attributes.value("node").toInt(), 3);
            }
        }
        xml.readNext();
    }
}
