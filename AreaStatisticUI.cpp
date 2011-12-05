#include "AreaStatisticUI.h"



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


}


/// 改变字段值
void AreaStatisticUI::changeFieldValue(const QString &fieldValue)
{


}