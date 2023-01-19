using OpenCvSharp;
using RemoteDesktop.Data;
using System;
using System.Net.Sockets;

namespace RemoteDesktop
{
    public class MyTcpBase
    {
        protected TcpClient client;
        protected NetworkStream stream;
        protected byte[] buffer;
        protected int port, bufferSize = 64000;
        protected string ip;

        protected System.Windows.Threading.DispatcherTimer timer;

        protected RemoteData remoteData = new RemoteData();
        protected Mat screenShotMat;

        public MyTcpBase(int _port, string _ip)
        {
            port = _port;
            ip = _ip;            
            buffer = new byte[bufferSize];
            timer = new System.Windows.Threading.DispatcherTimer();
            timer.Interval = new TimeSpan(0, 0, 0, 0, 20);            
        }

        ~MyTcpBase()
        {
            timer?.Stop();
        }
    }
}