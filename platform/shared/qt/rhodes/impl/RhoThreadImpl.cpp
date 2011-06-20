//#include <QThread>

extern "C" {

void* rho_nativethread_start()
{
    //TODO: rho_nativethread_start
    //QThread* thread = new QThread();
    //thread->start();
    //return (void*)thread;
    return 0;
}

void rho_nativethread_end(void* pData)
{
    //TODO: rho_nativethread_end
    //((QThread*)pData)->quit();
}

} //extern "C"
