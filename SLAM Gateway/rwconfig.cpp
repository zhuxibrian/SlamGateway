#include "rwconfig.h"


RWConfig::RWConfig(QObject *parent)
	: QObject(parent)
{
}

RWConfig::~RWConfig()
{
}

bool RWConfig::praseXML(QString strConfigXMLPath)
{
	bool res = false;
	if (strConfigXMLPath.isEmpty())
		return res;

	QDomDocument document;
	QString error;
	int row = 0, column = 0;
	QFile file(strConfigXMLPath);
	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::information(NULL, QString("parseXML"), QString("XML file open failed!"));
		return res;
	}

	if (!document.setContent(&file, false, &error, &row, &column)) {
		QMessageBox::information(NULL, QString("parseXML"), QString("parse file failed at line row and column") + QString::number(row, 10) + QString(",") + QString::number(column, 10));
		file.close();
		return res;
	}

	file.close();

	if (document.isNull())
	{
		QMessageBox::information(NULL, QString("parseXML"), QString("document is null!"));
		return res;
	}

	QDomElement root = document.documentElement();

	//xml版本号
	if (root.hasAttribute("version"))
		m_configVersion = root.attributeNode("version").value();


	//获取子节点
	QDomNodeList list = root.childNodes();
	int count = list.count();
	for (int i = 0; i<count; i++)
	{
		QDomNode dom_node = list.item(i);
		QDomElement element = dom_node.toElement();

		if (element.tagName() == "mqttBroker")
			praseMqttConfig(dom_node);
		else if (element.tagName() == "slams")
			praseSlamList(dom_node);
		else if (element.tagName() == "slamSubMessages")
			praseSlamMessages(dom_node);
		else if (element.tagName() == "ctrlSubMessages")
			praseCtrlMessages(dom_node);
	}

	return true;
}

/*解析mqtt代理服务器配置参数*/
void RWConfig::praseMqttConfig(QDomNode& node)
{
	QDomNamedNodeMap attributes = node.attributes();
	for (int i=0; i<attributes.size(); i++)
	{
		QDomNode attributeNode = attributes.item(i);
		m_mqttConfig[attributeNode.nodeName()] = attributeNode.nodeValue();
	}
}

/*解析所有slam车配置参数*/
void RWConfig::praseSlamList(QDomNode& node)
{
	QDomNodeList slamNodeList = node.childNodes();
	for (int i=0; i<slamNodeList.size(); i++)
	{
		QDomNode slamNode = slamNodeList.item(i);
		QDomNamedNodeMap slamAttributes = slamNode.attributes();
		SlamConfig slamConfig;
		for (int j=0; j<slamAttributes.size(); j++)
		{
			QDomNode attributeNode = slamAttributes.item(j);
			slamConfig[attributeNode.nodeName()] = attributeNode.nodeValue();
		}
		m_slamList.append(slamConfig);
	}
}

/*解析slam 模块支持的子命令（move等）*/
void RWConfig::praseSlamMessages(QDomNode& node)
{
	QDomNodeList slamMesList = node.childNodes();
	for (int i=0; i<slamMesList.size(); i++)
	{
		QDomNode  mesNode = slamMesList.item(i);
		QDomElement mesElement = mesNode.toElement();
		if (mesElement.hasAttribute("name"))
			m_slamMessages.append(mesElement.attributeNode("name").value());
	}
}

/*解析ctrl 模块支持的自命令（chargeConnect等）*/
void RWConfig::praseCtrlMessages(QDomNode& node)
{
	QDomNodeList ctrlMesList = node.childNodes();
	for (int i = 0; i < ctrlMesList.size(); i++)
	{
		QDomNode  mesNode = ctrlMesList.item(i);
		QDomElement mesElement = mesNode.toElement();
		if (mesElement.hasAttribute("name"))
			m_ctrlMessages.append(mesElement.attributeNode("name").value());
	}
}

