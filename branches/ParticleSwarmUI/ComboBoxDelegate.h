#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QStyledItemDelegate>
#include <QStandardItemModel>

#include <QComboBox>
#include <QWheelEvent>

class ComboBoxNoWheel : public QComboBox
{
	Q_OBJECT
public:
	ComboBoxNoWheel( QWidget * parent = 0 );
	~ComboBoxNoWheel();
	bool event ( QEvent * event );
};


class ComboBoxDelegateUValues : public QStyledItemDelegate
{
	Q_OBJECT
public:
	ComboBoxDelegateUValues(QStandardItemModel *pListModel , QStandardItemModel *pTableModel , QWidget *parent = 0  );
	
	QWidget *createEditor( QWidget *parent , const QStyleOptionViewItem &option ,const QModelIndex &index ) const;
	
	void setEditorData( QWidget *editor , const QModelIndex &index ) const;
	
	void setModelData( QWidget *editor , QAbstractItemModel *model ,const QModelIndex &index ) const;

private :
	const QStandardItemModel * const m_pListModel;
	const QStandardItemModel * const m_pTableModel;
};


class ComboBoxDelegateRange : public QStyledItemDelegate
{
	Q_OBJECT
public:
	ComboBoxDelegateRange(QStandardItemModel *pListModel , QStandardItemModel *pTableModel , QWidget *parent = 0  );
	
	QWidget *createEditor( QWidget *parent , const QStyleOptionViewItem &option ,const QModelIndex &index ) const;
	
	void setEditorData( QWidget *editor , const QModelIndex &index ) const;
	
	void setModelData( QWidget *editor , QAbstractItemModel *model ,const QModelIndex &index ) const;

private :
	const QStandardItemModel * const m_pListModel;
	const QStandardItemModel * const m_pTableModel;
};
#endif