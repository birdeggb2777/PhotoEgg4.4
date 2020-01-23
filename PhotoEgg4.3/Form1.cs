using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PhotoEgg4._3
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        int Mouse_X0 = 0, Mouse_Y0 = 0;//原始座標
        int Mouse_X1 = 0, Mouse_Y1 = 0;//先前座標
        int Mouse_Y2 = 0, Mouse_X2 = 0;//當前座標
        int Main_Width, Main_Height;//螢幕長寬
        Bitmap Main_Bitmap;//螢幕畫面
        Graphics g;
        private void Form1_Load(object sender, EventArgs e)
        {
            Form F = new Form();
            F.Show();
            F.WindowState = FormWindowState.Maximized;
            F.FormBorderStyle = FormBorderStyle.FixedSingle;
            Main_Width = F.Width - 20; Main_Height = F.Height - 20;
            F.Dispose();
             this.FormBorderStyle = 0;//無邊框
            this.WindowState = FormWindowState.Maximized;//最大化
            this.IsMdiContainer = true;
            this.Width = Main_Width;
            this.Height = Main_Height;
            this.Location = new Point(0, 0);

            TopForm topform = new TopForm(this);
            topform.MdiParent = this;
            topform.FormBorderStyle = 0;
            topform.Width = this.Width;          
            topform.Height = 35;
            topform.Location = new Point(0, 0);
            topform.Show();     
           

            DrawForm drawform = new DrawForm();
            drawform.MdiParent = this;
           
            drawform.FormBorderStyle = 0;
            drawform.Width = this.Width-10;/* /2 */;
            drawform.Height = this.Height - topform.Height-50;
            drawform.Show();
            drawform.Location = new Point(0, topform.Height);
           
           
           
            /* openFileDialog1.Filter = "圖片檔 (*.png;*.jpg;*.bmp;*.gif;*.tif)|*.png;*.jpg;*.bmp;*.gif;*.tif";
             if (openFileDialog1.ShowDialog() == DialogResult.OK)
             {
                 Main_Bitmap = new Bitmap(openFileDialog1.FileName);
 ;
                 g = this.CreateGraphics();
                 g.DrawImage(Main_Bitmap, new PointF(0, 0));
             }*/
        }
        private void Form1_MouseMove(object sender, MouseEventArgs e)
        {
            /*    try
                {
                    Mouse_X2 = e.X;
                    Mouse_Y2 = e.Y;
                    g.DrawImage(Main_Bitmap, new PointF(Mouse_X2, Mouse_Y2));
                    Mouse_X1 = e.X;
                    Mouse_Y1 = e.Y;
                }
                catch 
                {

                }*/
        }
    }
}
