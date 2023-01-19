using NAudio.CoreAudioApi;
using NAudio.Wave;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.ExceptionServices;
using System.Security;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Controls;
using Vosk;

namespace NewVoskTest
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private MMDeviceEnumerator enumerator;
        private MMDeviceCollection microphonesDevices;
        private WaveInEvent waveIn;
        private WaveFormat waveFormat = new WaveFormat(44100, 1);

        private Model model;
        private VoskRecognizer voskRec;
        private bool withPartial = false, onlySymbols = false;

        public MainWindow()
        {
            InitializeComponent();
            Loaded += MainWindow_Loaded;
            Closed += MainWindow_Closed;
        }

        private void MainWindow_Closed(object sender, EventArgs e)
        {
            StopAll();
            Process.GetCurrentProcess().Kill();
        }

        private void StopAll()
        {
            if (waveIn != null)
            {
                waveIn.DataAvailable -= WaveIn_DataAvailable;
                waveIn.StopRecording();
            }
        }

        [HandleProcessCorruptedStateExceptions]
        [SecurityCritical]
        private void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            Encoding.RegisterProvider(CodePagesEncodingProvider.Instance);

            enumerator = new MMDeviceEnumerator();
            microphonesDevices = enumerator.EnumerateAudioEndPoints(DataFlow.Capture, DeviceState.Active);
            foreach (var device in microphonesDevices)
            {
                var menuItem = new ComboBoxItem();
                menuItem.Content = device.FriendlyName.ToLower();
                MicrophonesComboBox.Items.Add(menuItem);
            }

            model = new Model("model");
            string[] arrayGrammar = new string[3] { "привет", "один", "[unk]" };
            var serialized = Newtonsoft.Json.JsonConvert.SerializeObject(arrayGrammar);
            try
            {
                voskRec = new VoskRecognizer(model, 44100.0f); // "[\"а о и е у ы л р ж з с привет\"]"
            }
            catch (System.AccessViolationException exception)
            {
                Application.Current.Dispatcher.Invoke(new Action(() =>
                {
                    RecognitionText.Text = exception.Message;
                }));
            }
        }

        private void MicrophonesComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            StopAll();
            RecognitionText.Text = "";
            ComboBox comboBox = (ComboBox)sender;
            ComboBoxItem selectedItem = (ComboBoxItem)comboBox.SelectedItem;
            var microphoneName = selectedItem.Content.ToString();
            for (int deviceIndex = 0; deviceIndex < WaveIn.DeviceCount; deviceIndex++)
            {
                foreach (var device in microphonesDevices)
                {
                    WaveInCapabilities deviceInfo = WaveIn.GetCapabilities(deviceIndex);
                    if (device.FriendlyName.ToLower().StartsWith(deviceInfo.ProductName.ToLower()))
                    {
                        if (device.FriendlyName.ToLower().StartsWith(microphoneName))
                        {
                            waveIn = new WaveInEvent
                            {
                                BufferMilliseconds = 100,
                                DeviceNumber = deviceIndex,
                                WaveFormat = waveFormat
                            };
                            waveIn.DataAvailable += WaveIn_DataAvailable;
                            waveIn.StartRecording();
                            return;
                        }
                    }
                }
            }
        }

        private void WaveIn_DataAvailable(object sender, WaveInEventArgs data)
        {
            try
            {
                if (voskRec.AcceptWaveform(data.Buffer, data.BytesRecorded))
                {
                    string voskResult = voskRec.Result();
                    Debug.WriteLine($"voskResult = {voskResult}");
                    List<string> result = ParseResults(voskResult);
                    for (var i = 0; i < result.Count; ++i)
                    {
                        if (!string.IsNullOrEmpty(result[i]))
                        {
                            Application.Current.Dispatcher.Invoke(new Action(() =>
                            {
                                RecognitionText.Text += result[i] + " ";
                            }));
                        }
                    }
                }
                else
                {
                    if (withPartial)
                    {
                        string voskPartialResult = voskRec.PartialResult();
                        List<string> partialResult = ParsePartialResults(voskPartialResult);
                        for (var i = 0; i < partialResult.Count; ++i)
                        {
                            if (!string.IsNullOrEmpty(partialResult[i]))
                            {
                                Application.Current.Dispatcher.Invoke(new Action(() =>
                                {
                                    RecognitionText.Text += partialResult[i] + " ";
                                }));
                            }
                        }
                    }
                }
            }
            catch (Exception exception)
            {
                Application.Current.Dispatcher.Invoke(new Action(() =>
                {
                    RecognitionText.Text = exception.Message;
                }));
            }
        }

        private List<string> ParseResults(String results)
        {
            var bytes = Encoding.Default.GetBytes(results);
            var input = Encoding.UTF8.GetString(bytes);
            var regex = new Regex("\"word\"\\s*:\\s*\"([^\"]*)\"");
            var matches = regex.Matches(input);
            var regexConf = new Regex("\"conf\"\\s*:\\s*([^,\n\"]*)");
            var matchesConf = regexConf.Matches(input);
            List<string> result = new List<string>();
            if (matches.Count != matchesConf.Count)
                return result;
            for (var i = 0; i < matches.Count; ++i)
            {
                int conf = (int)(Convert.ToDouble(matchesConf[i].Groups[1].Value.Replace(".", ",")) * 100);
                result.Add(matches[i].Groups[1].Value + ";" + conf.ToString());
            }
            return result;
        }

        private List<string> ParsePartialResults(String results)
        {
            var bytes = Encoding.Default.GetBytes(results);
            var input = Encoding.UTF8.GetString(bytes);
            var regex = new Regex("\"partial\"\\s*:\\s*\"([^\"]*)\"");
            var matches = regex.Matches(input);

            List<string> result = new List<string>();
            for (var i = 0; i < matches.Count; ++i)
            {
                result.Add(matches[i].Groups[1].Value);
            }
            return result;
        }

        private void Partial_Unchecked(object sender, RoutedEventArgs e)
        {
            withPartial = !withPartial;
        }

        private void Partial_Checked(object sender, RoutedEventArgs e)
        {
            withPartial = !withPartial;
        }

        private void OnlySymbols_Checked(object sender, RoutedEventArgs e)
        {
            onlySymbols = !onlySymbols;
        }

        private void ClearButton_Click(object sender, RoutedEventArgs e)
        {
            RecognitionText.Text = "";
        }

        private void OnlySymbols_Unchecked(object sender, RoutedEventArgs e)
        {
            onlySymbols = !onlySymbols;
        }
    }
}