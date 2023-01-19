using OpenCvSharp;
using RemoteDesktop.Data;
using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace RemoteDesktop
{
    public class MyTcpServer : MyTcpBase
    {
        private TcpListener server;
        private string receivedString;
        private int numBytesRead;

        public event EventHandler<Mat> ImageReady;

        public MyTcpServer(int _port, string _ip) : base(_port, _ip)
        {
            IPAddress networkAddr = IPAddress.Parse(ip);
            server = new TcpListener(networkAddr, port);
            server.Start();
            timer.Tick += ReceiveTimer_Tick;
            timer.Start();
        }

        private void ReceiveTimer_Tick(object sender, EventArgs e)
        {
            try
            {
                GetData();
                ParseData();
                if (!string.IsNullOrEmpty(receivedString))
                {
                    PrepareImage();
                    ImageReady?.Invoke(this, screenShotMat);
                }
            }
            catch { }
        }

        private void GetData()
        {
            client = server.AcceptTcpClient();
            stream = client.GetStream();
            receivedString = "";
            while ((numBytesRead = stream.Read(buffer, 0, buffer.Length)) > 0)
            {
                receivedString += Encoding.UTF8.GetString(buffer, 0, numBytesRead);
            }
            stream?.Close();
            client?.Close();
        }

        private void ParseData()
        {
            remoteData = Newtonsoft.Json.JsonConvert.DeserializeObject<RemoteData>(receivedString);
        }

        private void PrepareImage()
        {
            if (screenShotMat == null)
                screenShotMat = new Mat(remoteData.height, remoteData.width, MatType.CV_8UC3);
            screenShotMat = Cv2.ImDecode(remoteData.screenShotBytes, ImreadModes.Color);
        }
    }
}