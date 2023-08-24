#ifndef CfgINPUTDIALOG_H
#define CfgINPUTDIALOG_H
#include <QtGui>
#include <QDialog>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
class CfgInputDialog : public QDialog
{
 Q_OBJECT
private:
     QComboBox *strategy;
     QSpinBox *stations;
     QSpinBox *distance;
     QPushButton *m_OKButton;
     QPushButton *m_CancelButton;
    public:
    CfgInputDialog(QWidget *parent = 0);
    virtual ~CfgInputDialog();
    virtual void accept(); //{ QDialog::accept(); }
    virtual void reject(){ QDialog::reject(); }
signals:
    void passed();
};
#endif // QMULTIINPUTDIALOG_H
