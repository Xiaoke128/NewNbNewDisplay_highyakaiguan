namespace UartUpgradeTool
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.ComPort = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.OpenCom = new System.Windows.Forms.Button();
            this.SelectBoard = new System.Windows.Forms.ComboBox();
            this.FilePath = new System.Windows.Forms.TextBox();
            this.SelectFile = new System.Windows.Forms.Button();
            this.Start = new System.Windows.Forms.Button();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.timer2 = new System.Windows.Forms.Timer(this.components);
            this.AddrCombox = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.BaudRateCombox = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // ComPort
            // 
            this.ComPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ComPort.FormattingEnabled = true;
            this.ComPort.Location = new System.Drawing.Point(47, 12);
            this.ComPort.Name = "ComPort";
            this.ComPort.Size = new System.Drawing.Size(74, 20);
            this.ComPort.TabIndex = 0;
            this.ComPort.DropDown += new System.EventHandler(this.ComPort_DropDown);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 12);
            this.label1.TabIndex = 1;
            this.label1.Text = "Com:";
            // 
            // OpenCom
            // 
            this.OpenCom.Location = new System.Drawing.Point(477, 10);
            this.OpenCom.Name = "OpenCom";
            this.OpenCom.Size = new System.Drawing.Size(75, 23);
            this.OpenCom.TabIndex = 2;
            this.OpenCom.Text = "打开";
            this.OpenCom.UseVisualStyleBackColor = true;
            this.OpenCom.Click += new System.EventHandler(this.OpenCom_Click);
            // 
            // SelectBoard
            // 
            this.SelectBoard.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.SelectBoard.FormattingEnabled = true;
            this.SelectBoard.Items.AddRange(new object[] {
            "MainBoardFile",
            "DisplayBoardFile"});
            this.SelectBoard.Location = new System.Drawing.Point(14, 101);
            this.SelectBoard.Name = "SelectBoard";
            this.SelectBoard.Size = new System.Drawing.Size(107, 20);
            this.SelectBoard.TabIndex = 3;
            // 
            // FilePath
            // 
            this.FilePath.Location = new System.Drawing.Point(14, 57);
            this.FilePath.Name = "FilePath";
            this.FilePath.Size = new System.Drawing.Size(447, 21);
            this.FilePath.TabIndex = 4;
            // 
            // SelectFile
            // 
            this.SelectFile.Location = new System.Drawing.Point(477, 55);
            this.SelectFile.Name = "SelectFile";
            this.SelectFile.Size = new System.Drawing.Size(75, 23);
            this.SelectFile.TabIndex = 5;
            this.SelectFile.Text = "浏览";
            this.SelectFile.UseVisualStyleBackColor = true;
            this.SelectFile.Click += new System.EventHandler(this.SelectFile_Click);
            // 
            // Start
            // 
            this.Start.Location = new System.Drawing.Point(477, 99);
            this.Start.Name = "Start";
            this.Start.Size = new System.Drawing.Size(75, 23);
            this.Start.TabIndex = 6;
            this.Start.Text = "开始";
            this.Start.UseVisualStyleBackColor = true;
            this.Start.Click += new System.EventHandler(this.Start_Click);
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(14, 171);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(538, 23);
            this.progressBar1.TabIndex = 7;
            // 
            // timer2
            // 
            this.timer2.Enabled = true;
            this.timer2.Interval = 5;
            this.timer2.Tick += new System.EventHandler(this.timer2_Tick);
            // 
            // AddrCombox
            // 
            this.AddrCombox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.AddrCombox.FormattingEnabled = true;
            this.AddrCombox.Location = new System.Drawing.Point(205, 101);
            this.AddrCombox.Name = "AddrCombox";
            this.AddrCombox.Size = new System.Drawing.Size(70, 20);
            this.AddrCombox.TabIndex = 8;
            this.AddrCombox.SelectedIndexChanged += new System.EventHandler(this.AddrCombox_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(158, 104);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 12);
            this.label2.TabIndex = 9;
            this.label2.Text = "地址：";
            // 
            // BaudRateCombox
            // 
            this.BaudRateCombox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.BaudRateCombox.FormattingEnabled = true;
            this.BaudRateCombox.Items.AddRange(new object[] {
            "9600",
            "38400",
            "57600",
            "115200"});
            this.BaudRateCombox.Location = new System.Drawing.Point(205, 12);
            this.BaudRateCombox.Name = "BaudRateCombox";
            this.BaudRateCombox.Size = new System.Drawing.Size(70, 20);
            this.BaudRateCombox.TabIndex = 10;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(146, 15);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 12);
            this.label3.TabIndex = 11;
            this.label3.Text = "波特率：";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(564, 210);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.BaudRateCombox);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.AddrCombox);
            this.Controls.Add(this.progressBar1);
            this.Controls.Add(this.Start);
            this.Controls.Add(this.SelectFile);
            this.Controls.Add(this.FilePath);
            this.Controls.Add(this.SelectBoard);
            this.Controls.Add(this.OpenCom);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.ComPort);
            this.Name = "Form1";
            this.Text = "UpgradeTool";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox ComPort;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button OpenCom;
        private System.Windows.Forms.ComboBox SelectBoard;
        private System.Windows.Forms.TextBox FilePath;
        private System.Windows.Forms.Button SelectFile;
        private System.Windows.Forms.Button Start;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.Timer timer2;
        private System.Windows.Forms.ComboBox AddrCombox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox BaudRateCombox;
        private System.Windows.Forms.Label label3;
    }
}

