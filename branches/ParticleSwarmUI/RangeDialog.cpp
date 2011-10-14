#include "RangeDialog.h"
#include <QMessageBox>
#include <QStringList>
#include <QTextCodec>

RangeDialog::RangeDialog(  QStandardItemModel *pListModel , QStandardItemModel *pTableModel , QWidget * parent , Qt::WindowFlags f ) : 
                         QDialog( parent , f ) ,m_pListModel( pListModel ) , m_pTableModel( pTableModel )
{
	QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
	setupUi( this );

	m_RangeLowerOK = false ;
	m_RangeUpperOK = false ;

	RangeLower = 0;
	RangeUpper = 0;
	
    populateComboFromListModel( *pComboBox , *m_pListModel );
	populateComboFromListModel( *pComboBox2 , *m_pListModel );

	connect( pLERangeLower, SIGNAL( textEdited ( const QString& )  ), this, SLOT( verifyRangeLower( const QString& ) ) );
	connect( pLERangeUpper, SIGNAL( textEdited ( const QString& )  ), this, SLOT( verifyRangeUpper( const QString& ) ) );
	connect( pButnVerifyRange, SIGNAL( clicked()  ), this, SLOT( verifyButton() ) );
}


void RangeDialog::verifyRangeLower( const QString& str )
{
	bool OK;
	RangeLower = str.toDouble( &OK );
	if( OK )
	{
		m_RangeLowerOK = true ;
	}
	else
	{
		if( str != "" ) 
		{
			QMessageBox::information(0, tr("警告"), tr("不是正确的实数!"), QMessageBox::Ok);
		}
		m_RangeLowerOK = false ; 
	}
}

void RangeDialog::verifyRangeUpper( const QString& str )
{
	bool OK;
	RangeUpper = str.toDouble( &OK );
	if( OK )
	{
		m_RangeUpperOK = true ;
	}
	else
	{
		if( str != "" ) 
		{
			QMessageBox::information(0, tr("警告"), tr("不是正确的实数!"), QMessageBox::Ok);
		}
		m_RangeUpperOK = false ; 
	}
}

bool RangeDialog::verifyRange()
{
	if( m_RangeLowerOK && m_RangeUpperOK )
	{
		if( RangeLower <= RangeUpper )
		{
			return true;
		}
		else
		{
			return false;
		}
	}else
	{
		return false;
	}
}

void RangeDialog::verifyButton()
{
	if( verifyRange() )
	{
		QMessageBox::information(0, tr("OK"), tr("OK!"), QMessageBox::Ok);
	}
	else
	{
		QMessageBox::information(0, tr("警告"), tr("区间设置不正确!"), QMessageBox::Ok);
	}
}

void RangeDialog::populateComboFromListModel( QComboBox& rComboBox , const QStandardItemModel& rListModel)
{
	rComboBox.clear();

	QStringList strList;
	int i;
	int nCountItem = rListModel.rowCount();
	for( i = 0 ; i < nCountItem ; i++ )
	{
		strList.append( ( rListModel.item( i ) )->text() );
	}
	rComboBox.addItems( strList ); 
}