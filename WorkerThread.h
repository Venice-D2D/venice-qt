
#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QtCore/QThread>

class WorkerThread : public QThread
{
    Q_OBJECT
    void run() override {
        QString result;
        /* ... here is the expensive or blocking operation ... */
        emit resultReady(result);
    }
signals:
    void resultReady(const QString &s);
};

#endif // WORKERTHREAD_H
