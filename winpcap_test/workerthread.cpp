#include <QString>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <QDebug>
#include <QByteArray>
#include <QThread>
#include <QList>
#include <QVector>
#include <QMutex>
#include <QMutexLocker>
#include <QCoreApplication>

#include "workerthread.h"
#include "danmuwindow.h"
#include "Util.h"
using namespace std;
bool threadEndFlag;
QMutex m1;
QList<QString> messStack;
QMutex m2;
char errbuf[PCAP_ERRBUF_SIZE];
QMutex m3;
pcap_if_t *alldevs = NULL;
WorkerThread * wtList[30];
WorkerThread * mainThread = NULL;
int n;
QString WorkerThread::getFirstMessage()
{

        QMutexLocker ll(&m2);
        if (messStack.size()==0) return "";
        QString s = messStack.front();
        messStack.pop_front();
        return s;
}
bool WorkerThread::isWorking()
{
    bool ok = true;
    for (int i=1; i<=n; i++) {
        if (wtList[i]->isRunning()) {
            ok = false;
            break;
        }
    }
    if (ok) {
        QMutexLocker ll(&m3);
        for (int i=1; i<=n; i++) {
            delete wtList[i];
        }
        n = 0;
        if (alldevs!= NULL) {
            pcap_freealldevs(alldevs);
            alldevs = NULL;
        }
        return false;
    }
    return true;
}

bool WorkerThread::startDetecting()
{
    if (mainThread == NULL) mainThread = new WorkerThread;
    if (mainThread->isRunning()) {
        DanmuWindow::outputDebugInfor(myTr("监测模块忙，请稍后重试"));
        return false;
    }
    DanmuWindow::outputDebugInfor(myTr("监测模块启动中"));
    mainThread->jobFlag = 2;
    mainThread->start();
    return true;
}

bool WorkerThread::stopDetecting()
{
    if (mainThread == NULL) mainThread = new WorkerThread;
    if (mainThread->isRunning()) {
        DanmuWindow::outputDebugInfor(myTr("监测模块忙，请稍后重试"));
        return false;
    }
    DanmuWindow::outputDebugInfor(myTr("监测模块停止中"));
    mainThread->jobFlag = 1;
    mainThread->start();
    return true;
}


//获取本地IP地址
QString getIp(pcap_if_t *d)
{
    char  revIP[30];
    memset(revIP,	0,	30);

    pcap_addr *paddr = d->addresses;
    //paddr->addr->
    sockaddr_in *sin;
    QString re;
    for(	;paddr;	paddr = paddr->next)
    {
        sin = (struct sockaddr_in *)paddr->addr;
        switch(sin->sin_family){
        case	AF_INET:
            printf("IPV4 ");
            strcpy(revIP,	inet_ntoa(sin->sin_addr));
            re = QString(revIP);
            printf(" address : %s  %d\n",	revIP, sin->sin_addr);
            break;
        case	AF_INET6:
            printf("IPV6 ");
            strcpy(revIP,	inet_ntoa(sin->sin_addr));
            printf(" address : %s  %d\n",	revIP, sin->sin_addr);
            break;
        }

    }
    return re;
}
void WorkerThread::stopWork()
{
    if (WorkerThread::isWorking()) {
        //DanmuWindow::outputDebugInfor(myTr("停止监测模块"));
    }
    else {
        //DanmuWindow::outputDebugInfor(myTr("监测模块未运行，无法停止"));
        return;
    }
    {
        QMutexLocker ll(&m1);
        threadEndFlag = true;
    }
    while(WorkerThread::isWorking()) {
        msleep(100);
    }

}

void WorkerThread::startWork()
{
    stopWork();
    {
        QMutexLocker ll(&m1);
        threadEndFlag = false;
    }

    pcap_if_t *alldevs;
    pcap_if_t *d;

    int i=0;
    pcap_t *adhandle;


    /* Retrieve the device list */
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL /* auth is not needed */, &alldevs, errbuf) == -1)
    {
        fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
        return ;
    }

    /* Print the list */
    {
        QMutexLocker ll(&m3);
        for(d=alldevs; d; d=d->next)
        {
            printf("%d. %s", ++i, d->name);
            if (d->description)
                printf(" (%s)\n", d->description);
            else
                printf(" (No description available)\n");
            wtList[i] = new WorkerThread(0);
            wtList[i]->devs= d;
        }
    }
    if(i==0)
    {
        printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
        return ;
    }
    n = i;
    for (i=1; i<=n; i++) {
        wtList[i]->start();
    }

    msleep(1000);
    return ;
    /* Jump to the selected adapter */
    return ;

    /* Open the device */
    /* Open the adapter */
    if ((adhandle= pcap_open_live(d->name,	// name of the device
                             65536,			// portion of the packet to capture.
                                            // 65536 grants that the whole packet will be captured on all the MACs.
                             1,				// promiscuous mode (nonzero means promiscuous)
                             1000,			// read timeout
                             errbuf			// error buffer
                             )) == NULL)
    {
        fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
        /* Free the device list */
        pcap_freealldevs(alldevs);
        return ;
    }
    printf("%d", pcap_datalink(adhandle));
    printf("\nlistening on %s...\n", d->description);

    /* At this point, we don't need any more the device list. Free it */
    pcap_freealldevs(alldevs);

    /* start the capture */
    //pcap_loop(adhandle, 0, packet_handler, NULL);

    //pcap_close(adhandle);

}

void WorkerThread::work()
{
    qDebug()<<"work"<<endl;
    u_int netmask;
    char packet_filter[] = "tcp";
    pcap_t *adhandle;
    struct bpf_program fcode;
    cout<<"!1"<<endl;
    char errbuf[PCAP_ERRBUF_SIZE];
    if ((adhandle=  pcap_open(devs->name,	// name of the device
                             65536,			// portion of the packet to capture.
                                            // 65536 grants that the whole packet will be captured on all the MACs.
                             PCAP_OPENFLAG_PROMISCUOUS   ,
                             1000,			// read timeout
                              NULL,
                             errbuf			// error buffer
                             )) == NULL)
    {
        fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", devs->name);
        /* Free the device list */
        return ;
    }
    QString ip = getIp(devs);

    if (ip == "0.0.0.0") {
       // cout<< "invalid !!!"<<endl;
        return;
    }
    if(devs->addresses != NULL)
        /* 获得接口第一个地址的掩码 */
        netmask=((struct sockaddr_in *)(devs->addresses->netmask))->sin_addr.S_un.S_addr;
    else
        /* 如果接口没有地址，那么我们假设一个C类的掩码 */
        netmask=0xffffff;
  //  cout<<netmask<<endl;

    QString filter = "tcp and (dst host ";
    filter += ip;
    filter += " )";
    char*  ch;
    QByteArray ba = filter.toLatin1();
    ch=ba.data();
    cout<<"filter string " << ch<<endl;
    if (pcap_compile(adhandle, &fcode, ch, 1, netmask) <0 )
    {
        fprintf(stderr,"\nUnable to compile the packet filter. Check the syntax.\n");
        /* 释放设备列表 */
        pcap_freealldevs(alldevs);
        return ;
    }

    //设置过滤器
    //if (pcap_setfilter(adhandle, &fcode)<0)
    if  (false)
    {
        fprintf(stderr,"\nError setting the filter.\n");
        /* 释放设备列表 */
        pcap_freealldevs(alldevs);
        return ;
    }


    /* start the capture */
    //pcap_loop(adhandle, 0, packet_handler, NULL);
    cout<<"!2"<<endl;
    int count = 0;
    while(true) {
        {
            QMutexLocker ll(&m1);
            if (threadEndFlag) break;
        }
        struct pcap_pkthdr *header;
        const u_char *pkt_data;
        int r = pcap_next_ex(adhandle,&header,&pkt_data);
        count++;
        if (count%1000 == 0) {
          //  cout<<"receive data pack "<<count<<endl;
            //count = 0;
        }
        if (r!=1) continue;


        struct tm *ltime;
        char timestr[16];
        time_t local_tv_sec;

        /*
         * unused parameters
         */

        //(VOID)(pkt_data);
        //printf("%s",pkt_data);
        //QString s((const char *) pkt_data);
        //locate the tcp packet
        u_char a[5] = { 'c','m','d','i','d'};
        int length = min(header->len,200);
        int tt = search(pkt_data,  pkt_data+ length,a,a+5) - pkt_data;

        if (tt == length) continue;
        u_char b[2] = { '\'', '{'};
        tt = search(pkt_data, pkt_data+header->len,b,b+2) - pkt_data;
        QByteArray  ba;
        for (int i= tt+1; i<header->len; i++) {
            //cout<<pkt_data[i];
            ba.append(pkt_data[i]);
        }
        //cout<<endl<<endl;

        QString sss = QString::fromUtf8(ba);
        {
          //  if (sss.indexOf("getuc") < 0)
            //    if (sss.indexOf("chatmessage") < 0)
            //   if (sss.indexOf("Gift.Use") >= 0)
           // qDebug()<<sss<<endl;
        }
        //qDebug()<<"raw string  "<<sss <<endl;
        {
            QMutexLocker ll(&m2);
            messStack.push_back(sss);
        }
        continue;
        return;

        /* convert the timestamp to readable format */
        local_tv_sec = header->ts.tv_sec;
        ltime=localtime(&local_tv_sec);
        strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);

        printf("%s,%.6d len:%d\n", timestr, header->ts.tv_usec, header->len);


    }
    pcap_close(adhandle);

}

void WorkerThread::run()  {
    qDebug()<<"run "<<jobFlag<<endl;
    switch (jobFlag) {
        case 1:
            stopWork();
            break;
        case 2:
            startWork();
            break;
        case 3:
            work();
            break;
    }

}


void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

/* Callback function invoked by libpcap for every incoming packet */
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
    struct tm *ltime;
    char timestr[16];
    time_t local_tv_sec;

    /*
     * unused parameters
     */
    (VOID)(param);
    //(VOID)(pkt_data);
   // printf("%s",pkt_data);
    //QString s((const char *) pkt_data);
    //int r= header->len;
    u_char a[5] = { 'c','m','d','i','d'};

    int tt = search(pkt_data, pkt_data+min(header->len,200),a,a+5) - pkt_data;

    if (tt == header->len) return;
    u_char b[2] = { '\'', '{'};
    tt = search(pkt_data, pkt_data+header->len,b,b+2) - pkt_data;
    QByteArray  ba;
    for (int i= tt+1; i<header->len; i++) {
        cout<<pkt_data[i];
        ba.append(pkt_data[i]);
    }
    cout<<endl<<endl;
    qDebug()<<QString::fromUtf8(ba)<<endl;
    return;

    /* convert the timestamp to readable format */
    local_tv_sec = header->ts.tv_sec;
    ltime=localtime(&local_tv_sec);
    strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);

    printf("%s,%.6d len:%d\n", timestr, header->ts.tv_usec, header->len);

}


