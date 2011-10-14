#include "ComboBoxDelegate.h"

#include <QStringList>
#include <QStandardItem>


ComboBoxNoWheel::ComboBoxNoWheel( QWidget * parent ) : QComboBox( parent ) 
{}

ComboBoxNoWheel::~ComboBoxNoWheel()
{}
	
bool ComboBoxNoWheel::event ( QEvent * event )
{
	if( event->type() == QEvent::Wheel )
	{
		return true;
	}
	else
	{
		return QComboBox::event( event );
	}
}

/****************************************************/

ComboBoxDelegateUValues::ComboBoxDelegateUValues( QStandardItemModel *pListModel , QStandardItemModel *pTableModel, QWidget *parent ): 
                                           QStyledItemDelegate(parent) ,m_pListModel( pListModel ) , m_pTableModel( pTableModel )
{
}

QWidget* ComboBoxDelegateUValues::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if( index.column() == 1 )
	{
		if( m_pListModel != NULL )
		{
			int i;
			int nCountItem = m_pListModel->rowCount();

			QStandardItem *pStandardItem;
			
			QStringList strListItem;

			for( i = 0 ; i < nCountItem ; i++ )
			{
				pStandardItem = m_pListModel->item( i );
				strListItem.append( pStandardItem->text() );
			}
			
			ComboBoxNoWheel *pComboBox = new ComboBoxNoWheel( parent );

			pComboBox->addItems( strListItem );

			return pComboBox;
		
		}else
		{
			return NULL;
		}

	}
	else
	{
		return NULL;
	}
}

void ComboBoxDelegateUValues::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	if( editor != NULL)
	{
		if( index.column() == 1 )
		{
			ComboBoxNoWheel *pComboBox = static_cast<ComboBoxNoWheel *>( editor );
			QString strTemp = ( index.data() ).toString();
			pComboBox->setCurrentIndex( pComboBox->findText( strTemp ) );
		}
	}
}

void ComboBoxDelegateUValues::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	if( editor != NULL )
	{
		if( index.column() == 1 )
		{
			QStandardItem *pStandardItem;
			ComboBoxNoWheel *pComboBox = static_cast<ComboBoxNoWheel *>( editor );
			pStandardItem = m_pTableModel->item( index.row() , 1 ) ;
			pStandardItem->setText( pComboBox->currentText() );
		}
	}
}


ComboBoxDelegateRange::ComboBoxDelegateRange( QStandardItemModel *pListModel , QStandardItemModel *pTableModel, QWidget *parent ): 
                                           QStyledItemDelegate(parent) ,m_pListModel( pListModel ) , m_pTableModel( pTableModel )
{
}

QWidget* ComboBoxDelegateRange::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if( index.column() == 0 )
	{
		if( m_pListModel != NULL )
		{
			int i;
			int nCountItem = m_pListModel->rowCount();

			QStandardItem *pStandardItem;
			
			QStringList strListItem;

			for( i = 0 ; i < nCountItem ; i++ )
			{
				pStandardItem = m_pListModel->item( i );
				strListItem.append( pStandardItem->text() );
			}
			
			ComboBoxNoWheel *pComboBox = new ComboBoxNoWheel( parent );

			pComboBox->addItems( strListItem );

			return pComboBox;
		
		}else
		{
			return NULL;
		}

	}
	else
	{
		return QStyledItemDelegate::createEditor( parent , option , index );
	}
}

void ComboBoxDelegateRange::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	if( editor != NULL)
	{
		if( index.column() == 0 )
		{
			ComboBoxNoWheel *pComboBox = static_cast<ComboBoxNoWheel *>( editor );
			QString strTemp = ( index.data() ).toString();
			pComboBox->setCurrentIndex( pComboBox->findText( strTemp ) );
		}
		else
		{
			QStyledItemDelegate::setEditorData( editor , index );
		}
	}
}

void ComboBoxDelegateRange::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	if( editor != NULL )
	{
		if( index.column() == 0 )
		{
			QStandardItem *pStandardItem;
			ComboBoxNoWheel *pComboBox = static_cast<ComboBoxNoWheel *>( editor );
			pStandardItem = m_pTableModel->item( index.row() , 0 ) ;
			pStandardItem->setText( pComboBox->currentText() );
		}
		else
		{
			QStyledItemDelegate::setModelData( editor , model , index );
		}
	}
}
