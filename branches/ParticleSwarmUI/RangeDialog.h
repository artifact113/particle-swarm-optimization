#ifndef RANGEDIALOG_H
#define RANGEDIALOG_H

#include "ui_RangeDialog.h"

#include <QStandardItemModel>
#include <QComboBox>

class QDialog;

class RangeDialog : public QDialog , public Ui::RangeDialog
{
	Q_OBJECT
public:
	RangeDialog( QStandardItemModel *pListModel , QStandardItemModel *pTableModel , QWidget * parent = 0, Qt::WindowFlags f = 0  );
	
	double RangeLower;
	double RangeUpper;
	bool verifyRange();

private slots:
	void verifyButton();

	void verifyRangeLower( const QString& str );
	void verifyRangeUpper( const QString& str );
private:
	void populateComboFromListModel( QComboBox& rComboBox , const QStandardItemModel& rListModel);
	
	const QStandardItemModel * const m_pListModel;
	const QStandardItemModel * const m_pTableModel;

	bool m_RangeLowerOK;
	bool m_RangeUpperOK;

};

#endif