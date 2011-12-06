#include "AreaStatisticUI.h"
#include "ShapefileOperator.h"


/// 构造函数
AreaStatisticUI::AreaStatisticUI(const QString &dataSource, const QString &fieldName, QDomElement* parameter,QWidget* parent)
:QWidget(parent), mDataSource(dataSource), mFiledName(fieldName), mParameter(parameter)
{
	setupUi(this);
	initData();

	connect(cmbFieldValue, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(changeFieldValue(const QString &)));
}


/// 析构函数
AreaStatisticUI::~AreaStatisticUI()
{

}


/// 数据初始化
void AreaStatisticUI::initData()
{
	if (mDataSource.isEmpty() || mFiledName.isEmpty())
	{
		return;
	}

	// 填充cmbFieldValue
	vector<QString> uniqueValues(ShapefileReader::GetUniqueValues(mDataSource, mFiledName));
	vector<QString>::iterator iter;
	for (iter = uniqueValues.begin(); iter != uniqueValues.end(); ++iter)
	{
		this->cmbFieldValue->addItem(*iter);
	}

	// 读取配置
	if (mParameter->hasChildNodes())
	{
		QDomElement myElement = mParameter->childNodes().at(0).toElement();
		int currentIndex = this->cmbFieldValue->findText(myElement.attribute("text"));
		this->cmbFieldValue->setCurrentIndex(currentIndex);
	}
	else
	{
		QDomDocument myDocument(mParameter->ownerDocument());
		QDomElement myElement(myDocument.createElement("cmbFieldValue"));
		myElement.setAttribute("text", "");
	}
}


/// 改变字段值
void AreaStatisticUI::changeFieldValue(const QString &fieldValue)
{
	if (!fieldValue.isEmpty())
	{
		QDomElement myElement = mParameter->childNodes().at(0).toElement();
		myElement.setAttribute("text", fieldValue);
	}

}