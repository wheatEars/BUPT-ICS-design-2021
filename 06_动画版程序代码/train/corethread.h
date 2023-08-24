#ifndef CORETHREAD_H
#define CORETHREAD_H
#include <QThread>

class CoreThread : public QThread
{
public:
    CoreThread(QObject *parent = Q_NULLPTR);
    void run();
};

#endif // CORETHREAD_H
