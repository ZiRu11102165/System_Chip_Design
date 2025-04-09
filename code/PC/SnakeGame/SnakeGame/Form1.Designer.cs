namespace SnakeGame
{
    partial class Form1
    {
        /// <summary>
        /// 設計工具所需的變數。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清除任何使用中的資源。
        /// </summary>
        /// <param name="disposing">如果應該處置受控資源則為 true，否則為 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 設計工具產生的程式碼

        /// <summary>
        /// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
        /// 這個方法的內容。
        /// </summary>
        private void InitializeComponent()
        {
            this.gfoodtext = new System.Windows.Forms.Label();
            this.bfoodtext = new System.Windows.Forms.Label();
            this.gfood = new System.Windows.Forms.Label();
            this.bfood = new System.Windows.Forms.Label();
            this.SerialPortSelect = new System.Windows.Forms.ComboBox();
            this.SerialPortConnect = new System.Windows.Forms.Button();
            this.SerialPortDisconnect = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // gfoodtext
            // 
            this.gfoodtext.AutoSize = true;
            this.gfoodtext.Font = new System.Drawing.Font("新細明體", 14F);
            this.gfoodtext.Location = new System.Drawing.Point(41, 108);
            this.gfoodtext.Name = "gfoodtext";
            this.gfoodtext.Size = new System.Drawing.Size(129, 24);
            this.gfoodtext.TabIndex = 1;
            this.gfoodtext.Text = "Good Food : ";
            // 
            // bfoodtext
            // 
            this.bfoodtext.AutoSize = true;
            this.bfoodtext.Font = new System.Drawing.Font("新細明體", 14F);
            this.bfoodtext.Location = new System.Drawing.Point(56, 180);
            this.bfoodtext.Name = "bfoodtext";
            this.bfoodtext.Size = new System.Drawing.Size(110, 24);
            this.bfoodtext.TabIndex = 2;
            this.bfoodtext.Text = "Bad Food :";
            // 
            // gfood
            // 
            this.gfood.BackColor = System.Drawing.Color.Green;
            this.gfood.Location = new System.Drawing.Point(176, 108);
            this.gfood.Name = "gfood";
            this.gfood.Size = new System.Drawing.Size(25, 25);
            this.gfood.TabIndex = 3;
            // 
            // bfood
            // 
            this.bfood.BackColor = System.Drawing.Color.Red;
            this.bfood.Location = new System.Drawing.Point(176, 186);
            this.bfood.Name = "bfood";
            this.bfood.Size = new System.Drawing.Size(25, 25);
            this.bfood.TabIndex = 4;
            // 
            // SerialPortSelect
            // 
            this.SerialPortSelect.FormattingEnabled = true;
            this.SerialPortSelect.Location = new System.Drawing.Point(895, 40);
            this.SerialPortSelect.Name = "SerialPortSelect";
            this.SerialPortSelect.Size = new System.Drawing.Size(152, 23);
            this.SerialPortSelect.TabIndex = 5;
            this.SerialPortSelect.SelectedIndexChanged += new System.EventHandler(this.SerialPortSelect_SelectedIndexChanged);
            // 
            // SerialPortConnect
            // 
            this.SerialPortConnect.Font = new System.Drawing.Font("新細明體", 14F);
            this.SerialPortConnect.Location = new System.Drawing.Point(895, 82);
            this.SerialPortConnect.Name = "SerialPortConnect";
            this.SerialPortConnect.Size = new System.Drawing.Size(153, 66);
            this.SerialPortConnect.TabIndex = 6;
            this.SerialPortConnect.Text = "Connect";
            this.SerialPortConnect.UseVisualStyleBackColor = true;
            this.SerialPortConnect.Click += new System.EventHandler(this.SerialPortConnect_Click);
            // 
            // SerialPortDisconnect
            // 
            this.SerialPortDisconnect.Font = new System.Drawing.Font("新細明體", 14F);
            this.SerialPortDisconnect.Location = new System.Drawing.Point(895, 161);
            this.SerialPortDisconnect.Name = "SerialPortDisconnect";
            this.SerialPortDisconnect.Size = new System.Drawing.Size(153, 72);
            this.SerialPortDisconnect.TabIndex = 7;
            this.SerialPortDisconnect.Text = "Disconnect";
            this.SerialPortDisconnect.UseVisualStyleBackColor = true;
            this.SerialPortDisconnect.Click += new System.EventHandler(this.SerialPortDisconnect_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1082, 553);
            this.Controls.Add(this.SerialPortDisconnect);
            this.Controls.Add(this.SerialPortConnect);
            this.Controls.Add(this.SerialPortSelect);
            this.Controls.Add(this.bfood);
            this.Controls.Add(this.gfood);
            this.Controls.Add(this.bfoodtext);
            this.Controls.Add(this.gfoodtext);
            this.Name = "Form1";
            this.Text = "Snake Game";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label gfoodtext;
        private System.Windows.Forms.Label bfoodtext;
        private System.Windows.Forms.Label gfood;
        private System.Windows.Forms.Label bfood;
        private System.Windows.Forms.ComboBox SerialPortSelect;
        private System.Windows.Forms.Button SerialPortConnect;
        private System.Windows.Forms.Button SerialPortDisconnect;
    }
}

