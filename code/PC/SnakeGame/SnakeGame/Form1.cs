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

namespace SnakeGame
{
    public partial class Form1 : Form
    {
        const int ROW = 16, COL = 16;
        const int GRID_WIDTH = 25, GRID_HEIGHT = 25;
        const int GAME_POS_X = 200, GAME_POS_Y = 20;

        public Task ReceiveSerial;

        StringBuilder port = new StringBuilder();
        SerialPort SerialPort = new SerialPort();

        Color WIN = Color.Yellow;
        Color CONTINUE = Color.Yellow;
        Color LOSE = Color.Green;
        Color BG_COLOR = Color.Black;

        Color BORDER = Color.Gray;
        Color GFOOD = Color.Green;
        Color BFOOD = Color.Red;
        Color EMPTY = Color.Black;

        Color[,] GridsColor = new Color[ROW, COL];
        Label[,] grids = new Label[ROW, COL];

        
        int score = 0;

        public Form1()
        {
            InitializeComponent();
            SerialPortSelect.Items.AddRange(SerialPort.GetPortNames());

            for (int i = 0; i < ROW; i++)
            {
                for (int j = 0; j < COL; j++)
                {
                    grids[i, j] = new Label();
                    grids[i, j].Width = GRID_WIDTH;
                    grids[i, j].Height = GRID_HEIGHT;
                    
                    grids[i, j].BackColor = BG_COLOR;
                    grids[i, j].Left = j * GRID_WIDTH + GAME_POS_X;
                    grids[i, j].Top = i * GRID_HEIGHT + GAME_POS_Y;
                    grids[i, j].Visible = true;

                    this.Controls.Add(grids[i, j]);
                }
            }

            SerialPortDisconnect.Enabled = false;
            
        }

        private void SerialPortSelect_SelectedIndexChanged(object sender, EventArgs e)
        {
            port.Append(SerialPortSelect.Text);
        }

        private void SerialPortConnect_Click(object sender, EventArgs e)
        {
            try
            {
                SerialPort.PortName = port.ToString();
                SerialPort.BaudRate = 115200;
                SerialPort.Parity = Parity.None;
                SerialPort.DataBits = 8;
                SerialPort.StopBits = StopBits.One;
                SerialPort.Open();
                SerialPortConnect.Enabled = false;
                SerialPortDisconnect.Enabled = true;

                if (SerialPort.IsOpen)
                {
                    Receive_Data();
                }
            }
            catch(Exception exex)
            {
                MessageBox.Show(exex.Message);
                SerialPort.Close();

            }
        }

        private void SerialPortDisconnect_Click(object sender, EventArgs e)
        {
            SerialPortSelect.Enabled = true;
            SerialPortConnect.Enabled = false;
            SerialPort.Close();
        }

        private void Receive_Data()
        {
            try { 
                ReceiveSerial = new Task(Receive_Data_avilable);

                ReceiveSerial.Start();
            }
            catch(Exception exec)
            {
                MessageBox.Show(exec.Message);
            }
        }

        private void Receive_Data_avilable()
        {
            int row = 0, col = 0 ,i = 0;
            //int number = 256;
            int number = 256;
            byte[] ReceiveData = new byte[number];

            while (SerialPort.IsOpen)
            {
                SerialPort.DiscardInBuffer();
                //MessageBox.Show("check serialportselect again");
                //Task.Delay(1).Wait();
                while (SerialPort.BytesToRead != (256));
                    Task.Delay(5);

                SerialPort.Read(ReceiveData, 0, SerialPort.BytesToRead);

                for (i = 0; i < number; i++)
                {
                    
                    switch (ReceiveData[i])
                    {
                        case 0:
                            GridsColor[row, col] = EMPTY;
                            break;
                        case 1:
                            GridsColor[row, col] = BORDER;
                            break;
                        case 2:
                            GridsColor[row, col] = CONTINUE;
                            break;
                        case 3:
                            GridsColor[row, col] = GFOOD;
                            break;
                        case 4:
                            GridsColor[row, col] = BFOOD;
                            break;
                        default:
                            break;
                    }
                    Console.Write($"{ReceiveData[i]}");
                    col = col + 1;
                    if (col == COL)
                    {
                        row = row + 1;
                        col = 0;
                        Console.WriteLine();
                    }
                    if (row == ROW)
                    {
                        row = 0;
                        col = 0;

                    }
                }
                //Console.WriteLine($"{ Convert.ToString(ReceiveData[256],16)}");
                //SerialPort.DiscardInBuffer();
                UpadteScreen();
                
                //SerialPort.DiscardInBuffer();
            }
        }
        private void UpadteScreen()
        {
            //if(grids.InvokeRequired)
            this.Invoke(new EventHandler(delegate
            {
                for (int i = 0; i < ROW; i++)
                {
                    for (int j = 0; j < COL; j++)
                    {
                        grids[i, j].BackColor = GridsColor[i, j];
                    }
                }
            }));
        }
    }
}
