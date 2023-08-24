#include <QtGui>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QWidget>
#include <QPushButton>
#include "cfginputdialog.h"
#include <QComboBox>
#include <QSpinBox>
#include <QMessageBox>
#include "heads.h"
#include "statetable.h"

CfgInputDialog::CfgInputDialog( QWidget *parent)
: QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *subLayout;
    //设计界⾯
    subLayout = new QHBoxLayout;
    QLabel *strategylabel = new QLabel(this);
    strategylabel->setText("策略");
    subLayout->addWidget(strategylabel);
    strategy = new QComboBox(this);
    strategy->addItem("FCFS");
    strategy->addItem("SSTF");
    strategy->addItem("SCAN");
    subLayout->addWidget(strategy);
    layout->addLayout(subLayout);

    subLayout = new QHBoxLayout;
    QLabel *numlabel = new QLabel(this);
    numlabel->setText("站数");
    subLayout->addWidget(numlabel);
    stations = new QSpinBox(this);
    stations->setValue(5);
    subLayout->addWidget(stations);
    layout->addLayout(subLayout);

    subLayout = new QHBoxLayout;
    QLabel *dislabel = new QLabel(this);
    dislabel->setText("距离");
    subLayout->addWidget(dislabel);
    distance= new QSpinBox(this);
    distance->setValue(3);
    subLayout->addWidget(distance);
    layout->addLayout(subLayout);

    m_OKButton = new QPushButton( tr("OK"), this );
    m_CancelButton = new QPushButton( tr("Cancel"), this );
    subLayout = new QHBoxLayout;
    subLayout->addStretch();
    subLayout->addWidget(m_OKButton);
    subLayout->addWidget(m_CancelButton);
    layout->addLayout(subLayout);
    setLayout(layout);
    connect(m_OKButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}
CfgInputDialog::~CfgInputDialog()
{
}
void CfgInputDialog::accept(){
    if(stations->value()<=3){
        QMessageBox::warning(this,"输入错误","站数应不小于四");
    }
    else if(distance->value()<=0){
        QMessageBox::warning(this,"输入错误","请确保距离大于0");
    }
    else{
        cfg.DISTANCE=distance->value();
        cfg.TOTAL_STATION=stations->value();
        cfg.STRATEGY=strategy->currentIndex()+1;
        emit passed();
        qDebug()<<cfg.DISTANCE<< cfg.TOTAL_STATION<< cfg.STRATEGY;
        QDialog::accept();
    }
}
