using OpenCvSharp;
using System;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;

namespace RemoteDesktop
{
    public class MyTcpClient : MyTcpBase
    {
        private System.Drawing.Bitmap bitmap;
        private System.Drawing.Graphics graphics;
        private System.Drawing.Rectangle screenArea = System.Drawing.Rectangle.Empty;

        private Size resizedSize;
        private Mat resizedMat = new Mat();
        private byte[] screenShotBytes, resizedScreenShotBytes;

        private int frameCount = 0;
        public event EventHandler<int> SendedScreenShot;

        public MyTcpClient(int _port, string _ip) : base(_port, _ip)
        {
            screenArea = GetScreenArea();
            if (screenArea != System.Drawing.Rectangle.Empty)
            {
                screenShotBytes = new byte[screenArea.Width * screenArea.Height * 3];
                resizedSize = new Size(screenArea.Width / 2, screenArea.Height / 2);
                resizedScreenShotBytes = new byte[resizedSize.Width * resizedSize.Height * 3];
                bitmap = new System.Drawing.Bitmap(screenArea.Width, screenArea.Height);
                graphics = System.Drawing.Graphics.FromImage(bitmap);
            }
            timer.Tick += SenderTimer_Tick;
            timer.Start();
        }

        private void SenderTimer_Tick(object sender, EventArgs e)
        {
            try
            {
                PrepareData();
                SendData();
                SendedScreenShot?.Invoke(this, ++frameCount);
            }
            catch { }
        }

        private void PrepareData()
        {
            GetScreenShot();
            if (screenShotMat == null)
                screenShotMat = new Mat(screenArea.Height, screenArea.Width, MatType.CV_8UC3);
            Marshal.Copy(screenShotBytes, 0, screenShotMat.Data, screenShotBytes.Length);
            Cv2.Resize(screenShotMat, resizedMat, resizedSize);
            Cv2.ImEncode(".jpg", resizedMat, out resizedScreenShotBytes);
        }

        private void SendData()
        {
            client = new TcpClient();
            client.Connect(ip, port);
            stream = client.GetStream();
            remoteData.screenShotBytes = resizedScreenShotBytes;
            remoteData.width = resizedSize.Width;
            remoteData.height = resizedSize.Height;
            var serialized = Newtonsoft.Json.JsonConvert.SerializeObject(remoteData);
            var sendingBytes = Encoding.UTF8.GetBytes(serialized);
            if (stream.CanWrite)
                stream.Write(sendingBytes, 0, sendingBytes.Length);
            stream?.Close();
            client?.Close();
        }

        #region screen methods
        [StructLayout(LayoutKind.Sequential)]
        public struct CURSORINFO
        {
            public Int32 cbSize;
            public Int32 flags;
            public IntPtr hCursor;
            public POINTAPI ptScreenPos;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct POINTAPI
        {
            public int x;
            public int y;
        }

        [DllImport("user32.dll")]
        public static extern bool GetCursorInfo(out CURSORINFO pci);

        [DllImport("user32.dll")]
        public static extern bool DrawIcon(IntPtr hDC, int X, int Y, IntPtr hIcon);

        public const Int32 CURSOR_SHOWING = 0x00000001;
        protected CURSORINFO pci;

        private System.Drawing.Rectangle GetScreenArea()
        {
            int screenCount = System.Windows.Forms.Screen.AllScreens.Length;
            System.Drawing.Rectangle screenArea = System.Drawing.Rectangle.Empty;
            for (int i = 0; i < screenCount; ++i)
            {
                if (System.Windows.Forms.Screen.AllScreens[i].Primary == true)
                    screenArea = System.Windows.Forms.Screen.AllScreens[i].Bounds;
            }
            if (screenArea.Width == 1366 && screenArea.Height == 768)
                screenArea.Width = 1368;
            return screenArea;
        }

        private void GetScreenShot()
        {
            graphics.CopyFromScreen(0, 0, 0, 0, new System.Drawing.Size(screenArea.Width, screenArea.Height));
            pci.cbSize = Marshal.SizeOf(typeof(CURSORINFO));
            if (GetCursorInfo(out pci))
            {
                if (pci.flags == CURSOR_SHOWING)
                {
                    DrawIcon(graphics.GetHdc(), pci.ptScreenPos.x, pci.ptScreenPos.y, pci.hCursor);
                    graphics.ReleaseHdc();
                }
            }
            var bits = bitmap.LockBits(new System.Drawing.Rectangle(0, 0, screenArea.Width, screenArea.Height), System.Drawing.Imaging.ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
            Marshal.Copy(bits.Scan0, screenShotBytes, 0, screenShotBytes.Length);
            bitmap.UnlockBits(bits);
        }
        #endregion
    }
}