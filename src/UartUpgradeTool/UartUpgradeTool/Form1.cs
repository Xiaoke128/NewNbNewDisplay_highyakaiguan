using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;
using System.Threading;

namespace UartUpgradeTool
{
    public partial class Form1 : Form
    {
        SerialPort Sport = new SerialPort();
        bool openFlag = false;
        public Form1()
        {
            InitializeComponent();
            
        }
        System.Timers.Timer timeoutTimer = new System.Timers.Timer(500);
        System.Timers.Timer SendActTimer = new System.Timers.Timer(5);
        private void Form1_Load(object sender, EventArgs e)
        {
            BaudRateCombox.SelectedIndex = 3;
            Start.Enabled = false;
            SelectBoard.SelectedIndex = 0;
            string[] ports = System.IO.Ports.SerialPort.GetPortNames();//获取电脑上可用串口号
            ComPort.Items.AddRange(ports);
            ComPort.SelectedIndex = ComPort.Items.Count > 0 ? 0 : -1;
            Sport.DataReceived += new SerialDataReceivedEventHandler(rData);
            
            timeoutTimer.Elapsed += new System.Timers.ElapsedEventHandler(timer1_Tick);
            SendActTimer.Elapsed += new System.Timers.ElapsedEventHandler(SendTimer_Tick);
            timeoutTimer.AutoReset = true;
            SendActTimer.AutoReset = true;
            timer2.Start();
            string[] addrStr = new string[255];
            for(int i = 0; i < 255; i++)
            {
                addrStr[i] = (i + 1).ToString();
            }
            AddrCombox.Items.AddRange(addrStr);
            AddrCombox.SelectedIndex = 0;
            deviceAddr = 1;
        }
        int step = 0;
        bool responseSucc = false;
        bool timeOutFlag = false;

        private void timer1_Tick(object sender, EventArgs e)
        {
            //step = 0;
            //responseSucc = true;
            //timeOutFlag = true;
        }
        bool ableTimer = false;
        int intervalTier = 100;
        int tickCount = 0;
        private void timer2_Tick(object sender, EventArgs e)
        {
            if(ableTimer)
            {
                tickCount++;
                if(tickCount >= intervalTier)
                {
                    tickCount = 0;
                    ableTimer = false;
                    step = 0;
                    responseSucc = true;
                    timeOutFlag = true;
                }
            }
        }

        private void Timer2enable()
        {
            ableTimer = true;
            tickCount = 0;
        }

        private void Timer2disable()
        {
            ableTimer = false;
        }

        public void parseData(byte data)
        {
            //timeoutTimer.Enabled = false;
            //timeoutTimer.Enabled = true;
            Timer2enable();
            switch (step)
            {
                case 0:
                    if(data == 0x5A)
                    {
                        step++;
                    }
                    break;
                case 1:
                    if((data >= 0x81) && (data <= 0x85))
                    {
                        step++;
                    }
                    else
                    {
                        step = 0;
                    }
                    break;
                case 2:
                    if(data == 3)
                    {
                        step++;
                    }
                    else
                    {
                        step = 0;
                    }
                    break;
                case 3:
                    if (data == 0xff)
                    {
                        step++;
                    }
                    else
                    {
                        step = 0;
                    }
                    break;
                case 4:
                    if (data == 0xff)
                    {
                        step++;
                    }
                    else
                    {
                        step = 0;
                    }
                    break;
                case 5:
                    if (data == 0x5b)
                    {
                        responseSucc = true;
                    }
                    step = 0;
                    break;
                default:

                    break;
            }
        }
        byte[] RevData = new byte[500];
        int revIndex = 0;
        public void rData(object sender, SerialDataReceivedEventArgs e)
        {
            int len = Sport.BytesToRead;
            if (len > 0)
            {
                byte[] buf = new byte[len];
                Sport.Read(buf, 0, len);
                for(int i = 0; i < len; i++)
                {
                    //parseData(buf[i]);
                    RevData[revIndex++] = buf[i];
                }
            }
        }

        public void ClearRev()
        {
            revIndex = 0;
            for(int i = 0; i < RevData.Length; i++)
            {
                RevData[i] = 0;
            }
        }

        private void OpenCom_Click(object sender, EventArgs e)
        {
            if(OpenCom.Text == "打开")
            {
                try
                {
                    Sport.PortName = ComPort.Text;
                    Sport.BaudRate = Convert.ToInt32(BaudRateCombox.Text);
                    Sport.DataBits = 8;
                    Sport.StopBits = StopBits.One;
                    Sport.Parity = Parity.None;
                    Sport.Open();
                    OpenCom.Text = "关闭";
                    openFlag = true;
                    ComPort.Enabled = false;
                }
                catch(Exception err)
                {
                    MessageBox.Show("打开串口失败，请查看该串口是否已经被打开过。");
                }
            }
            else
            {
                try
                {
                    Sport.Close();
                    OpenCom.Text = "打开";
                    openFlag = false;
                    ComPort.Enabled = true;
                }
                catch (Exception)
                {
                    MessageBox.Show("关闭串口失败");
                }
            }
        }

        private void ComPort_DropDown(object sender, EventArgs e)
        {
            if(!openFlag)
            {
                ComPort.Items.Clear();
                string[] ports = System.IO.Ports.SerialPort.GetPortNames();//获取电脑上可用串口号
                ComPort.Items.AddRange(ports);
            }
        }
        string fileFullname;
        private void SelectFile_Click(object sender, EventArgs e)
        {
            
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.InitialDirectory = $"{AppDomain.CurrentDomain.BaseDirectory}help\\";
            ofd.Filter = "bin文件(*.pdf)|*.bin";
            ofd.RestoreDirectory = true;
            if (ofd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                //文件全名
                fileFullname = ofd.FileName;
                //获取文件路径的方法
                FilePath.Text = fileFullname;
                string path = Path.GetFileName(fileFullname); //只获取文件名
                string path12 = Path.GetDirectoryName(fileFullname);//获取文件路径
                Start.Enabled = true;
            }
            
        }

        public void SendExitBypass()
        {
            byte[] sendBuf = new byte[6];

            sendBuf[0] = (byte)'*';
            sendBuf[1] = (byte)'!';
            sendBuf[2] = (byte)'*';
            sendBuf[3] = (byte)'@';
            sendBuf[4] = (byte)'*';
            sendBuf[5] = (byte)'#';
            Sport.Write(sendBuf, 0, 6);
        }
        public void sendData(byte command, byte[] addr, byte[] data, int dataLen)
        {
            byte[] sendBuf = new byte[150];
            int i = 0;
            byte length = 3;

            sendBuf[i++] = 0x5A;
            sendBuf[i++] = deviceAddr;
            sendBuf[i++] = command;
            sendBuf[i++] = length;
            if (addr != null)
            {
                length += 4;
                for (int k = 0; k < 4; k++)
                {
                    sendBuf[i++] = addr[k];
                }
            }
            
            if(dataLen > 0)
            {
                length += (byte)dataLen;
                for (int j = 0; j < dataLen; j++)
                {
                    sendBuf[i++] = data[j];
                }
            }
            sendBuf[i++] = 0xff;
            sendBuf[i++] = 0xff;
            sendBuf[i++] = 0x5b;
            sendBuf[3] = length;
            Sport.Write(sendBuf, 0, i);
        }
        UInt32 address = 0x08004000;
        int boardIndex = 0;
        private void Start_Click(object sender, EventArgs e)
        {
            DialogResult result = MessageBox.Show("请确认已经选择正确的升级文件和设备地址！", "提示",MessageBoxButtons.OKCancel, MessageBoxIcon.Warning);
            if (result == DialogResult.OK)
            {
                Start.Enabled = false;
                boardIndex = SelectBoard.SelectedIndex;
                //SendActTimer.Interval = 20;
                SendActTimer.Enabled = true;
                if(boardIndex == 0)
                {
                    SendStep = 0;
                    sendFlag = false;
                }
                else
                {
                    SendStep = 5;
                    sendFlag = false;
                }
            }
            else
            {
                // 用户选择了“否”
                // 执行其他操作或者直接退出程序
            }
            
        }

        public void UpdateProcessBarVal(int val)
        {
            if (this.progressBar1.InvokeRequired)
            {
                this.progressBar1.Invoke(new Action<int>(value =>
                {
                    this.progressBar1.Value = value;
                }), val);
            }
            else
            {
                this.progressBar1.Value = val;
            }
        }

        public void UpdateStartButtonState(bool able)
        {
            if(this.Start.InvokeRequired)
            {
                this.Start.Invoke(new Action<bool>(value =>
                {
                    this.Start.Enabled = value;
                }), able);
            }
            else
            {
                this.Start.Enabled = able;
            }
        }

        int SendStep = 0;
        bool sendFlag = false;
        //bool fileOpenFlag = false;
        int file_len;
        byte[] binBuff = new byte[512];
        int readLen;
        byte[] addrBuf = new byte[4];
        BinaryReader br;
        int processVal = 0;
        int numPak = 0;
        int waitCount = 0;
        int delayCount = 0;
        int exitbypss = 0;
        private void SendTimer_Tick(object sender, EventArgs e)
        {
            bool flagTimer = true;

            SendActTimer.Enabled = false;
            switch(SendStep)
            {
                case 0:
                    //need add action to make device to bootloader
                    if (!sendFlag)
                    {
                        sendFlag = true;
                        ClearRev();
                        waitCount = 0;
                        byte[] tempIndex = new byte[5];
                        tempIndex[0] = (byte)boardIndex;
                        FileStream Myfile = new FileStream(fileFullname, FileMode.Open, FileAccess.Read);
                        br = new BinaryReader(Myfile);
                        file_len = (int)Myfile.Length;
                        processVal = (file_len / 128 + 1) / 98 + 1;
                        //fileOpenFlag = true;
                        numPak = 0;
                        if (file_len == 0)
                        {
                            SendStep = 0;
                            br.Close();
                            flagTimer = false;
                            UpdateStartButtonState(true);
                            MessageBox.Show("选取的文件为空！");
                        }
                        else
                        {
                            tempIndex[1] = (byte)(file_len >> 24);
                            tempIndex[2] = (byte)(file_len >> 16);
                            tempIndex[3] = (byte)(file_len >> 8);
                            tempIndex[4] = (byte)(file_len);
                            sendData(0x06, null, null, 0);
                        }
                        
                    }
                    else
                    {
                        waitCount++;
                        if ((revIndex >= 6) && (RevData[0] == 0x5A) && (RevData[5] == 0x5B))
                        {
                            waitCount = 0;
                            SendStep = 7;
                            sendFlag = false;
                        }
                        if (waitCount >= 100)
                        {
                            if (MessageBox.Show("通信连接未建立，是否继续？", "信息", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
                            {
                                SendStep++;
                            }
                            else
                            {
                                SendStep = 0;
                                
                                flagTimer = false;
                                UpdateStartButtonState(true);
                                
                            }
                            waitCount = 0;
                            sendFlag = false;
                        }
                    }
                    break;
                case 1:
                    if(!sendFlag)
                    {
                        sendFlag = true;
                        ClearRev();
                        waitCount = 0;
                        //SendActTimer.Interval = 20;
                        sendData(0x01, null,null, 0);
                    }
                    else
                    {
                        waitCount++;
                        if ((revIndex == 7)  && (RevData[0] == 0x5A) && (RevData[6] == 0x5B))
                        {
                            if(((boardIndex == 0) && (RevData[3] == 0x00)) || ((boardIndex == 1) && (RevData[3] == 0x01)))
                            {
                               
                                UpdateProcessBarVal(1);
                                SendStep++;
                                
                            }
                            else
                            {
                                SendStep = 0;
                                flagTimer = false;
                                UpdateStartButtonState(true);
                                br.Close();
                                MessageBox.Show("所选的主板型号不匹配。");
                            }
                            waitCount = 0;
                            sendFlag = false;
                        }
                        if(waitCount >= 100)
                        {
                            SendStep = 0;
                            waitCount = 0;
                            flagTimer = false;
                            UpdateStartButtonState(true);
                            sendFlag = false;
                            br.Close();
                            MessageBox.Show("获取不到设备信息");
                        }
                    }
                    break;
                case 2:
                    //if(!fileOpenFlag)
                    //{
                    //    FileStream Myfile = new FileStream(fileFullname, FileMode.Open, FileAccess.Read);
                    //    br = new BinaryReader(Myfile);
                    //    file_len = (int)Myfile.Length;
                    //    processVal = (file_len / 128 + 1) / 98 + 1;
                    //    fileOpenFlag = true;
                    //    numPak = 0;
                    //    if (file_len == 0)
                    //    {
                    //        SendStep = 0;
                    //        br.Close();
                    //        flagTimer = false;
                    //        UpdateStartButtonState(true);
                    //        MessageBox.Show("选取的文件为空！");
                    //    }
                    //}
                    //else
                    //{
                    if (!sendFlag)
                    {
                        sendFlag = true;
                        addrBuf[0] = (byte)(Convert.ToUInt32(address.ToString()) >> 24);
                        addrBuf[1] = (byte)(Convert.ToUInt32(address.ToString()) >> 16);
                        addrBuf[2] = (byte)(Convert.ToUInt32(address.ToString()) >> 8);
                        addrBuf[3] = (byte)(Convert.ToUInt32(address.ToString()));
                        ClearRev();
                        waitCount = 0;
                        //SendActTimer.Interval = 50;
                        sendData(0x02, addrBuf, null, 0);
                    }
                    else
                    {
                        waitCount++;
                        if ((revIndex == 6) && (RevData[0] == 0x5A) && (RevData[5] == 0x5B))
                        {
                            SendStep++;
                            sendFlag = false;
                        }
                        if (waitCount >= 100)
                        {

                            SendStep = 0;
                            flagTimer = false;
                            UpdateStartButtonState(true);
                            processVal = 0;
                            br.Close();
                            //fileOpenFlag = false;
                            MessageBox.Show("擦除失败！");
                            UpdateProcessBarVal(0);
                            sendFlag = false;
                        }
                    }
                    //}
                    break;
                case 3:
                    if (!sendFlag)
                    {
                        sendFlag = true;
                        //SendActTimer.Interval = 20;
                        if (file_len / 128 > 0)
                        {
                            readLen = 128;
                        }
                        else
                        {
                            readLen = file_len % 128;
                        }
                        addrBuf[0] = (byte)(Convert.ToUInt32(address.ToString()) >> 24);
                        addrBuf[1] = (byte)(Convert.ToUInt32(address.ToString()) >> 16);
                        addrBuf[2] = (byte)(Convert.ToUInt32(address.ToString()) >> 8);
                        addrBuf[3] = (byte)(Convert.ToUInt32(address.ToString()));
                        binBuff = br.ReadBytes(readLen);
                        ClearRev();
                        waitCount = 0;
                        sendData(0x03, addrBuf, binBuff, readLen);
                        numPak++;
                        //progressBar1.Value = 1 + numPak / processVal;
                        UpdateProcessBarVal(1 + numPak / processVal);
                    }
                    else
                    {
                        waitCount++;
                        if ((revIndex == 6) && (RevData[0] == 0x5A) && (RevData[5] == 0x5B))
                        {
                            sendFlag = false;
                            address += (UInt32)readLen;
                            file_len -= readLen;

                            if (file_len == 0)
                            {
                                SendStep = 4;
                                address = 0x08004000;
                                br.Close();
                                //fileOpenFlag = false;
                                processVal = 0;
                                UpdateProcessBarVal(99);
                            }
                            else
                            {
                                if (((address & 0x0000FFFF) % 0x400) == 0)
                                {
                                    SendStep = 2;
                                }
                            }
                        }
                        if (waitCount >= 100)
                        {
                            SendStep = 0;
                            address = 0x08004000;
                            br.Close();
                            //fileOpenFlag = false;
                            flagTimer = false;
                            UpdateStartButtonState(true);
                            processVal = 0;
                            MessageBox.Show("写入失败！");
                            UpdateProcessBarVal(0);
                            sendFlag = false;
                        }
                    }
                    break;
                case 4:
                    if (!sendFlag)
                    {
                        sendFlag = true;
                        ClearRev();
                        waitCount = 0;
                        //SendActTimer.Interval = 20;
                        sendData(0x05, null, null, 0);
                    }
                    else
                    {
                        waitCount++;
                        if ((revIndex >= 6) && (RevData[0] == 0x5A) && (RevData[5] == 0x5B))
                        {
                                                   
                            UpdateProcessBarVal(100);
                            sendFlag = false;
                            if (boardIndex == 1)
                            {
                                SendStep = 6;
                                flagTimer = true;
                            }
                            else
                            {
                                SendStep = 0;
                                UpdateStartButtonState(true);
                                flagTimer = false;
                                MessageBox.Show("升级成功！");
                                UpdateProcessBarVal(0);
                            }                                                        
                        }
                        if (waitCount >= 100)
                        {
                            SendStep = 0;
                            flagTimer = false;
                            UpdateStartButtonState(true);
                            MessageBox.Show("退出失败！");
                            UpdateProcessBarVal(0);
                            sendFlag = false;
                        }
                    }
                        break;
                case 5:
                    if (!sendFlag)
                    {
                        sendFlag = true;
                        ClearRev();
                        waitCount = 0;
                        //SendActTimer.Interval = 20;
                        sendData(0x07, null, null, 0);
                    }
                    else
                    {
                        waitCount++;
                        if ((revIndex >= 6) && (RevData[0] == 0x5A) && (RevData[5] == 0x5B))
                        {
                            SendStep = 0;
                            sendFlag = false;
                        }
                        if (waitCount >= 100)
                        {
                            SendStep = 0;
                            UpdateStartButtonState(true);
                            flagTimer = false;
                            sendFlag = false;
                            MessageBox.Show("和主板通信失败！");
                        }
                    }
                    break;
                case 6:
                    exitbypss++;
                    if(exitbypss >= 100)
                    {
                        exitbypss = 0;
                        SendExitBypass();
                        SendExitBypass();
                        SendExitBypass();
                        SendStep = 0;
                        UpdateStartButtonState(true);
                        flagTimer = false;
                        MessageBox.Show("升级成功！");
                        UpdateProcessBarVal(0);
                    }
                    
                    break;
                case 7:
                    delayCount++;
                    if(delayCount >= 400)
                    {
                        delayCount = 0;
                        SendStep = 1;
                    }
                    break;
                default:

                    break;

            }
            if(flagTimer)
            {
                SendActTimer.Enabled = true;
            }           
        }
        byte deviceAddr = 0;
        private void AddrCombox_SelectedIndexChanged(object sender, EventArgs e)
        {
            deviceAddr = (byte)(AddrCombox.SelectedIndex + 1);
        }
    }
}
