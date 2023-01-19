using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace RemoteDesktop
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private WriteableBitmap writeableBitmap;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void StartServerButton_Click(object sender, RoutedEventArgs e)
        {
            StartClientButton.Visibility = Visibility.Collapsed;
            StartServerButton.IsEnabled = false;
            StartServerButton.Content = "Listener";
            int port = int.Parse(PortTextBlock.Text);
            var tcp = new MyTcpServer(port, IpTextBlock.Text);
            tcp.ImageReady += Tcp_ImageReady;
        }

        private void Tcp_ImageReady(object sender, OpenCvSharp.Mat frame)
        {
            if (frame != null)
            {
                if (writeableBitmap == null)
                {
                    writeableBitmap = new WriteableBitmap(frame.Width, frame.Height, 96, 96, PixelFormats.Bgr24, null);
                    MainImage.Source = writeableBitmap;
                }
                writeableBitmap.WritePixels(new Int32Rect(0, 0, frame.Width, frame.Height),
                            frame.Data, frame.Width * frame.Height * frame.ElemSize(), frame.Width * frame.ElemSize());
            }
        }

        private void StartClientButton_Click(object sender, RoutedEventArgs e)
        {
            StartServerButton.Visibility = Visibility.Collapsed;
            StartClientButton.IsEnabled = false;
            StartClientButton.Content = "Sender";
            int port = int.Parse(PortTextBlock.Text);
            var tcp = new MyTcpClient(port, IpTextBlock.Text);
            ServerViewBox.Visibility = Visibility.Collapsed;
            DebugTextBox.Visibility = Visibility.Visible;
            tcp.SendedScreenShot += Tcp_SendedScreenShot;
        }

        private void Tcp_SendedScreenShot(object sender, int frameCount)
        {
            var len = DebugTextBox.Text.Length;
            if (len > 3000)
                DebugTextBox.Text = "";
            DebugTextBox.Text += frameCount + " ";
        }
    }
}