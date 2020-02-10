using PhotoEgg4._3.Properties;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using PenDrawing;
using CvDrawing;
namespace PhotoEgg4._3
{
    public partial class DrawForm : Form
    {
        public DrawForm()
        {
            InitializeComponent();
            // tabControl1.SelectedIndexChanged += new EventHandler(TabControl1_SelectedIndexChanged);
        }
        int Mouse_X0 = 0, Mouse_Y0 = 0;//原始座標
        int Mouse_X1 = 0, Mouse_Y1 = 0;//先前座標
        int Mouse_Y2 = 0, Mouse_X2 = 0;//當前座標
        int NowPicture_X = 0;
        int NowPicture_Y = 0;
        int Main_Width, Main_Height;//螢幕長寬
        bool MouseDownClick = false;
        Bitmap Main_Bitmap;//螢幕畫面
                           // Bitmap Display_Main_Bitmap;//螢幕畫面
        bool SelectColo_MouseDownClick = false;
        int selectColorX = 0;
        int selectColorY = 0;
        int selectFullColor = 0;
        double StretchRange = 1;
        Bitmap SelectColor_BMP = (Bitmap)Resources.chooseColor_Rec2;
        Bitmap chooseColor_Rec2_NowColor = Resources.chooseColor_Rec2;
        Graphics g;
        ImageList imageList1;
        DrawClass PixelDraw_Operate = new DrawClass();
        CvClass Cv_Operate = new CvClass();
        private void 載入圖片ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFileDialog1.Filter = "圖片檔 (*.png;*.jpg;*.bmp;*.gif;*.tif)|*.png;*.jpg;*.bmp;*.gif;*.tif";
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                StretchRange = 1;
                Main_Bitmap = new Bitmap(openFileDialog1.FileName);
                // Main_Bitmap.SetResolution(1,1);
                NowPicture_X = (this.Width - Main_Bitmap.Width) / 2; NowPicture_Y = (this.Height - Main_Bitmap.Height) / 2;

                if ((int)(Main_Bitmap.Height * StretchRange) > this.Height ||
                    (int)(Main_Bitmap.Width * StretchRange) > this.Width)
                {
                    while ((int)(Main_Bitmap.Height * StretchRange) > this.Height ||
                    (int)(Main_Bitmap.Width * StretchRange) > this.Width)
                    {
                        StretchRange /= 1.1;
                    }
                    NowPicture_X = (int)(Main_Bitmap.Width * StretchRange);
                    NowPicture_Y = 0;
                }
                //  MessageBox.Show("" + NowPicture_X);
                reflesh();

            }
        }

        private Graphics backGraphics;
        private Bitmap backBmp;
        private void DrawForm_Load(object sender, EventArgs e)
        {
            this.DoubleBuffered = true;
            this.backBmp = new Bitmap(this.DisplayRectangle.Width, this.DisplayRectangle.Height);
            this.backGraphics = Graphics.FromImage(backBmp);
            this.MouseWheel += new MouseEventHandler(MouseWheelHandler);
            this.Paint += new PaintEventHandler(Form_Paint);
            SelectRectColorPicture.Size = Resources.chooseColor_Rec2.Size;
            SelectFullColor_Picture.Size = Resources.ColorFull.Size;
        }
        private void MouseWheelHandler(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            // If the mouse wheel delta is positive, move the box up.
            double tempRange = StretchRange;
            if (e.Delta > 0)
            {
                StretchRange *= 1.1;
                NowPicture_X -=((int)(Main_Bitmap.Width * StretchRange)- (int)(Main_Bitmap.Width * tempRange))/2;
                NowPicture_Y -= ((int)(Main_Bitmap.Height * StretchRange) - (int)(Main_Bitmap.Height * tempRange))/2;
            }

            // If the mouse wheel delta is negative, move the box down.
            if (e.Delta < 0)
            {
                StretchRange /= 1.1;
                NowPicture_X -= ((int)(Main_Bitmap.Width * StretchRange) - (int)(Main_Bitmap.Width * tempRange)) / 2;
                NowPicture_Y -= ((int)(Main_Bitmap.Height * StretchRange) - (int)(Main_Bitmap.Height * tempRange)) / 2;
            }
            //  NowPicture_X = NowPicture_X+(int)(Main_Bitmap.Width - (Main_Bitmap.Width * StretchRange))/2;
            // NowPicture_Y = NowPicture_Y + (int)(Main_Bitmap.Height - (Main_Bitmap.Height * StretchRange)) / 2;
            try
            {
                reflesh();
            }
            catch { }

        }
        protected override void OnPaint(PaintEventArgs e)
        {
            // If there is an image and it has a location, 
            // paint it when the Form is repainted.
            base.OnPaint(e);

            try
            {
                // Create a local version of the graphics object for the PictureBox.
                //DrawBrush(Mouse_X1 - NowPicture_X, Mouse_Y1 - NowPicture_Y, Mouse_X2 - NowPicture_X, Mouse_Y2 - NowPicture_Y);
                reflesh();
            }
            catch { }
        }
        private void Form_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
        {
            try
            {
                // Create a local version of the graphics object for the PictureBox.
                //DrawBrush(Mouse_X1 - NowPicture_X, Mouse_Y1 - NowPicture_Y, Mouse_X2 - NowPicture_X, Mouse_Y2 - NowPicture_Y);
                reflesh();
            }
            catch { }
        }
        private void Form_MouseMove(object sender, MouseEventArgs e)
        {
            menuStrip1.Focus();
            try
            {
                Mouse_X2 = e.X;
                Mouse_Y2 = e.Y;
                if (MouseDownClick == true && tabControl1.SelectedTab == tabControl1.TabPages[1])
                {
                    this.backGraphics.FillRectangle(Brushes.White, 0, 0, this.DisplayRectangle.Width, this.DisplayRectangle.Height);
                    NowPicture_X += Mouse_X2 - Mouse_X1;
                    NowPicture_Y += Mouse_Y2 - Mouse_Y1;
                    // this.backGraphics.DrawImage(Main_Bitmap, new Rectangle(NowPicture_X, NowPicture_Y, (int)(Main_Bitmap.Width * StretchRange), (int)(Main_Bitmap.Height * StretchRange)));
                    //this.CreateGraphics().DrawImageUnscaled(this.backBmp, 0, 0);
                }
                if (tabControl1.SelectedTab == tabControl1.TabPages[2])
                {
                    if (MouseDownClick == true)
                    { // DrawBrush((Mouse_X1 - NowPicture_X) * (int)(Main_Bitmap.HorizontalResolution), Mouse_Y1 - NowPicture_Y, (Mouse_X2 - NowPicture_X) * (int)(Main_Bitmap.HorizontalResolution), Mouse_Y2 - NowPicture_Y);

                        DrawBrush(Mouse_X1 - NowPicture_X, Mouse_Y1 - NowPicture_Y, Mouse_X2 - NowPicture_X, Mouse_Y2 - NowPicture_Y);
                        this.backGraphics.FillRectangle(Brushes.White, 0, 0, this.DisplayRectangle.Width, this.DisplayRectangle.Height);
                        // this.backGraphics.DrawImage(Main_Bitmap, new Rectangle(NowPicture_X, NowPicture_Y, (int)(Main_Bitmap.Width * StretchRange), (int)(Main_Bitmap.Height * StretchRange)));
                        // this.CreateGraphics().DrawImageUnscaled(this.backBmp, 0, 0);
                    }
                    else
                    {
                        //  Pen myPen = new Pen(Color.Black);
                        //    this.backGraphics.DrawEllipse(myPen, NowPicture_X - 15, NowPicture_Y - 15, 30, 30);
                        //this.backGraphics.FillRectangle(Brushes.White, 0, 0, this.DisplayRectangle.Width, this.DisplayRectangle.Height);
                    }
                }

                reflesh();
                Mouse_X1 = e.X;
                Mouse_Y1 = e.Y;
            }
            catch
            {

            }
        }

        private void Form_MouseDown(object sender, MouseEventArgs e)
        {
            int size = 20;
            try
            {
                // size = Int32.Parse(PenSizeText1.Text);
                size = (int)(numericUpDown1.Value);
                PixelDraw_Operate.CreateBrush(size, (int)(numericUpDown3.Value));
            }
            catch { }
            try
            {
                unsafe
                {
                    Bitmap MyNewBmp = (Bitmap)Main_Bitmap;
                    Rectangle MyRec = new Rectangle(0, 0, MyNewBmp.Width, MyNewBmp.Height);
                    BitmapData MyBmpData = MyNewBmp.LockBits(MyRec, ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
                    PixelDraw_Operate.CreateUnSharpMark((byte*)(MyBmpData.Scan0), MyNewBmp.Width, MyNewBmp.Height, 4);
                    MyNewBmp.UnlockBits(MyBmpData);
                }
            }
            catch { }

            try { PixelDraw_Operate.CreateUnSharpMark(Main_Bitmap.Width, Main_Bitmap.Height); }
            catch { }
            try
            {
                Mouse_X2 = e.X;
                Mouse_Y2 = e.Y;
                MouseDownClick = true;
                Mouse_X1 = e.X;
                Mouse_Y1 = e.Y;
            }
            catch
            {

            }
            GC.Collect();
        }
        private void Form_MouseUp(object sender, MouseEventArgs e)
        {
            try
            {
                Mouse_X2 = e.X;
                Mouse_Y2 = e.Y;

                Mouse_X1 = e.X;
                Mouse_Y1 = e.Y;
                if (MouseDownClick == true && tabControl1.SelectedTab == tabControl1.TabPages[3])
                {

                    DrawOil(Mouse_X1 - NowPicture_X, Mouse_Y1 - NowPicture_Y, Mouse_X2 - NowPicture_X, Mouse_Y2 - NowPicture_Y);

                }

            }
            catch
            {
                MessageBox.Show("");
            }
            MouseDownClick = false;
            GC.Collect();
        }
        private void DrawOil(int OpointX, int OpointY, int pointX, int pointY)
        {

            int transparent = 100;
            try
            {
                // transparent = Int32.Parse(TransparentText.Text);
                transparent = (int)(numericUpDown2.Value);
                if (transparent > 100) transparent = 100;
            }
            catch { }
            int Line = (int)Math.Sqrt((double)(OpointX * OpointX + OpointY * OpointY));
            Bitmap MyNewBmp = (Bitmap)Main_Bitmap;
            Rectangle MyRec = new Rectangle(0, 0, MyNewBmp.Width, MyNewBmp.Height);
            BitmapData MyBmpData = MyNewBmp.LockBits(MyRec, ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);


            // PixelDraw_Operate.BlackBrush((byte*)(MyBmpData.Scan0), MyNewBmp.Width, MyNewBmp.Height, 4, (int)(50 / StretchRange), (int)(50 / StretchRange), transparent);

            int Reduce_X = OpointX > pointX ? pointX : OpointX;
            int Reduce_Y = OpointY > pointY ? pointY : OpointY;
            int Up_X = OpointX < pointX ? pointX : OpointX;
            int Up_Y = OpointY < pointY ? pointY : OpointY;
            int X = (int)(Reduce_X + (double)Math.Abs(OpointX - pointX) / (double)Line * 1);
            int Y = (int)(Up_Y + -(double)Math.Abs(OpointY - pointY) / (double)Line * 1);
            unsafe
            {
                Cv_Operate.FillOil((byte*)(MyBmpData.Scan0), MyNewBmp.Width, MyNewBmp.Height, 4, (int)(X / StretchRange), (int)(Y / StretchRange), (int)(numericUpDown4.Value), (byte)PixelDraw_Operate.Color_B, (byte)PixelDraw_Operate.Color_G, (byte)PixelDraw_Operate.Color_R);

                // PixelDraw_Operate.FillOil((byte*)(MyBmpData.Scan0), MyNewBmp.Width, MyNewBmp.Height, 4, (int)(X / StretchRange), (int)(Y / StretchRange), (int)(numericUpDown4.Value));
            }
            MyNewBmp.UnlockBits(MyBmpData);
            GC.Collect();
        }
        private void DrawBrush(int OpointX, int OpointY, int pointX, int pointY)
        {

            int transparent = 100;

            try
            {
                // transparent = Int32.Parse(TransparentText.Text);
                transparent = (int)(numericUpDown2.Value);
                if (transparent > 100) transparent = 100;
            }
            catch { }
            int Line = (int)Math.Sqrt((double)(OpointX * OpointX + OpointY * OpointY));
            Bitmap MyNewBmp = (Bitmap)Main_Bitmap;
            Rectangle MyRec = new Rectangle(0, 0, MyNewBmp.Width, MyNewBmp.Height);
            BitmapData MyBmpData = MyNewBmp.LockBits(MyRec, ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            unsafe
            {
                // PixelDraw_Operate.BlackBrush((byte*)(MyBmpData.Scan0), MyNewBmp.Width, MyNewBmp.Height, 4, (int)(50 / StretchRange), (int)(50 / StretchRange), transparent);

                int Reduce_X = OpointX > pointX ? pointX : OpointX;
                int Reduce_Y = OpointY > pointY ? pointY : OpointY;
                int Up_X = OpointX < pointX ? pointX : OpointX;
                int Up_Y = OpointY < pointY ? pointY : OpointY;
                if ((OpointX > pointX) != (OpointY > pointY))
                {
                    for (var i = 0; i < Line; i++)
                    {
                        int X = (int)(Reduce_X + (double)Math.Abs(OpointX - pointX) / (double)Line * i);
                        int Y = (int)(Up_Y + -(double)Math.Abs(OpointY - pointY) / (double)Line * i);
                        // PixelDraw_Operate.Black((byte*)(MyBmpData.Scan0), MyNewBmp.Width, MyNewBmp.Height, 4, (int)(X / StretchRange), (int)(Y / StretchRange), size, transparent);
                        PixelDraw_Operate.BlackBrush((byte*)(MyBmpData.Scan0), MyNewBmp.Width, MyNewBmp.Height, 4, (int)(X / StretchRange), (int)(Y / StretchRange), transparent);

                    }

                }
                else
                {
                    for (var i = 0; i < Line; i++)
                    {
                        int X = (int)(Reduce_X + (double)Math.Abs(OpointX - pointX) / (double)Line * i);
                        int Y = (int)(Reduce_Y + (double)Math.Abs(OpointY - pointY) / (double)Line * i);
                        // PixelDraw_Operate.Black((byte*)(MyBmpData.Scan0), MyNewBmp.Width, MyNewBmp.Height, 4, (int)(X / StretchRange), (int)(Y / StretchRange), size, transparent);
                        PixelDraw_Operate.BlackBrush((byte*)(MyBmpData.Scan0), MyNewBmp.Width, MyNewBmp.Height, 4, (int)(X / StretchRange), (int)(Y / StretchRange), transparent);

                    }
                }
            }
            MyNewBmp.UnlockBits(MyBmpData);
            GC.Collect();
        }
        /*  private void PenSizeText1_KeyPress(object sender, KeyPressEventArgs e)
          {
              if (((int)e.KeyChar < 48 | (int)e.KeyChar > 57) & (int)e.KeyChar != 8)
              {
                  e.Handled = true;
              }
          }
          private void TransparentText_KeyPress(object sender, KeyPressEventArgs e)
          {
              if (((int)e.KeyChar < 48 | (int)e.KeyChar > 57) & (int)e.KeyChar != 8)
              {
                  e.Handled = true;
              }
          }*/
        private void SelectRectColorPicture_MouseDown(object sender, MouseEventArgs e)
        {
            SelectColo_MouseDownClick = true;
            refleshSelectRectColorPicture(sender, e);
        }
        private void SelectRectColorPicture_MouseMove(object sender, MouseEventArgs e)
        {
            refleshSelectRectColorPicture(sender, e);
        }
        private void SelectRectColorPicture_MouseUp(object sender, MouseEventArgs e)
        {
            SelectColo_MouseDownClick = false;
            refleshSelectRectColorPicture(sender, e);
        }
        private void refleshSelectRectColorPicture(object sender, MouseEventArgs e)
        {

            if (SelectColo_MouseDownClick == false) return;
            Bitmap chooseColor_Rec2 = (Bitmap)chooseColor_Rec2_NowColor.Clone();
            Graphics chooseColor_Rec2_G = Graphics.FromImage(chooseColor_Rec2);
            // BufferedGraphicsContext currentContext = BufferedGraphicsManager.Current;
            // BufferedGraphics myBuffer = currentContext.Allocate(Graphics.FromImage(chooseColor_Rec2), SelectRectColorPicture.DisplayRectangle);

            //Graphics selectColor_G = pictureBox1.CreateGraphics(); ;
            // myBuffer.Graphics.Clear(Color.White);
            chooseColor_Rec2_G.DrawImage(chooseColor_Rec2_NowColor, new Rectangle(0, 0, chooseColor_Rec2.Width, chooseColor_Rec2.Height));
            try
            {
                // Color c = (SelectColor_BMP).GetPixel(e.X, e.Y);
                //  if (c.R == 255 && c.G == 255 && c.B == 255 && e.X > 5) return;
                selectColorX = e.X;
                selectColorY = e.Y;
                Pen myPen = new Pen(Color.Black);
                chooseColor_Rec2_G.DrawEllipse(myPen, selectColorX - 15, selectColorY - 15, 30, 30);
                myPen = new Pen(Color.White);
                chooseColor_Rec2_G.DrawEllipse(myPen, selectColorX - 16, selectColorY - 16, 32, 32);
                // myBuffer.Render(Graphics.FromImage(chooseColor_Rec2));
                Color c = (SelectColor_BMP).GetPixel(e.X, e.Y);
                PixelDraw_Operate.Color_R = c.R;
                PixelDraw_Operate.Color_G = c.G;
                PixelDraw_Operate.Color_B = c.B;
            }
            catch
            {

            }
            SelectRectColorPicture.Image = chooseColor_Rec2;
        }
        private void reflesh()
        {
            const int rect = 25;
            try
            {
                for (var j = 0; j < this.DisplayRectangle.Height; j += rect)
                {
                    for (var i = 0; i < this.DisplayRectangle.Width; i += rect * 2)
                    {
                        if (j % (rect * 2) == 0)
                        {
                            this.backGraphics.FillRectangle(Brushes.White, i, j, rect, rect);
                            this.backGraphics.FillRectangle(Brushes.Gray, i + rect, j, rect, rect);
                        }
                        else
                        {
                            this.backGraphics.FillRectangle(Brushes.White, i + rect, j, rect, rect);
                            this.backGraphics.FillRectangle(Brushes.Gray, i, j, rect, rect);
                        }
                    }
                }
                try
                {
                    this.backGraphics.DrawImage(Main_Bitmap, new Rectangle(NowPicture_X, NowPicture_Y, (int)(Main_Bitmap.Width * StretchRange), (int)(Main_Bitmap.Height * StretchRange)));
                }
                catch { }
                if (tabControl1.SelectedTab == tabControl1.TabPages[2])
                {
                    if (MouseDownClick == true)
                    { }
                    else
                    {
                        Pen myPen = new Pen(Color.Black);
                        this.backGraphics.DrawEllipse(myPen, Mouse_X2, Mouse_Y2, (int)(numericUpDown1.Value), (int)(numericUpDown1.Value));
                    }
                }
                this.CreateGraphics().DrawImageUnscaled(this.backBmp, 0, 0);
            }
            catch { }
        }
        private void RefleshSelectFull(object sender, MouseEventArgs e)
        {
            Bitmap ColorFullBitmap = Resources.ColorFull;
            Graphics SelectFullColor_G = Graphics.FromImage(ColorFullBitmap);//  SelectFullColor_Picture.CreateGraphics(); ;
            SelectFullColor_G.DrawImage(Resources.ColorFull, new Rectangle(0, 0, Resources.ColorFull.Width, Resources.ColorFull.Height));
            if (SelectFullColor_MouseDown == true)
            {
                Pen myPen = new Pen(Color.Black);
                SelectFullColor_G.DrawRectangle(myPen, 0, e.Y - 10, ColorFullBitmap.Width - 1, 20);
                SelectFullColor_Picture.Image = ColorFullBitmap;

                SelectColor_BMP = (Bitmap)Resources.chooseColor_Rec2;
                Bitmap MyNewBmp = (Bitmap)SelectColor_BMP;
                Rectangle MyRec = new Rectangle(0, 0, MyNewBmp.Width, MyNewBmp.Height);
                BitmapData MyBmpData = MyNewBmp.LockBits(MyRec, ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
                unsafe
                {
                    try
                    {
                        selectFullColor = e.X;
                        PixelDraw_Operate.ConvertHSV((byte*)(MyBmpData.Scan0), MyNewBmp.Width, MyNewBmp.Height, e.Y, 0, 0, 4, false, 0);
                    }
                    catch { }
                    finally { MyNewBmp.UnlockBits(MyBmpData); }
                }
                try
                {
                    Graphics selectColor_G = Graphics.FromImage(MyNewBmp);// pictureBox1.CreateGraphics();
                    selectColor_G.DrawImage(SelectColor_BMP, new Rectangle(0, 0, MyNewBmp.Width, MyNewBmp.Height));
                    selectColor_G.Dispose();
                    chooseColor_Rec2_NowColor = new Bitmap((Bitmap)MyNewBmp);

                    selectColor_G = Graphics.FromImage(MyNewBmp);
                    myPen = new Pen(Color.Black);
                    selectColor_G.DrawEllipse(myPen, selectColorX - 15, selectColorY - 15, 30, 30);
                    myPen = new Pen(Color.White);
                    selectColor_G.DrawEllipse(myPen, selectColorX - 16, selectColorY - 16, 32, 32);

                    SelectRectColorPicture.Image = MyNewBmp;

                    Color c = (SelectColor_BMP).GetPixel(selectColorX, selectColorY);
                    PixelDraw_Operate.Color_R = c.R;
                    PixelDraw_Operate.Color_G = c.G;
                    PixelDraw_Operate.Color_B = c.B;
                }
                catch {/* MessageBox.Show("error");*/ }
            }
            GC.Collect();
        }
        private void timer1_Tick(object sender, EventArgs e)
        {
            reflesh();

            /* SelectColor_BMP = (Bitmap)Resources.chooseColor_Rec2;

             Graphics selectColor_G = pictureBox1.CreateGraphics();
             selectColor_G.DrawImage(SelectColor_BMP, new Rectangle(0, 0, 255, 255));
             Pen myPen = new Pen(Color.Black);
             selectColor_G.DrawEllipse(myPen, selectColorX - 15, selectColorY - 15, 30, 30);
             myPen = new Pen(Color.White);
             selectColor_G.DrawEllipse(myPen, selectColorX - 16, selectColorY - 16, 32, 32);
             */
        }

        bool SelectFullColor_MouseDown = false;
        private void SelectFullColor_Picture_MouseDown(object sender, MouseEventArgs e)
        {
            SelectFullColor_MouseDown = true;
            RefleshSelectFull(sender, e);
        }
        private void SelectFullColor_Picture_MouseMove(object sender, MouseEventArgs e)
        {
            RefleshSelectFull(sender, e);
        }
        private void SelectFullColor_Picture_MouseUp(object sender, MouseEventArgs e)
        {
            SelectFullColor_MouseDown = false;
            RefleshSelectFull(sender, e);
            /* SelectColor_BMP = (Bitmap)Resources.chooseColor_Rec2;
             Bitmap MyNewBmp = (Bitmap)SelectColor_BMP;
             Rectangle MyRec = new Rectangle(0, 0, MyNewBmp.Width, MyNewBmp.Height);
             BitmapData MyBmpData = MyNewBmp.LockBits(MyRec, ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
             unsafe
             {
                 try
                 {
                     selectFullColor = e.X;
                     PixelDraw_Operate.ConvertHSV((byte*)(MyBmpData.Scan0), MyNewBmp.Width, MyNewBmp.Height, e.Y, 0, 0, 4, false, 0);
                 }
                 catch { }
                 finally { MyNewBmp.UnlockBits(MyBmpData); }
             }
             try
             {
                 Graphics selectColor_G = pictureBox1.CreateGraphics();
                 selectColor_G.DrawImage(SelectColor_BMP, new Rectangle(0, 0, 255, 255));
                 Pen myPen = new Pen(Color.Black);
                 selectColor_G.DrawEllipse(myPen, selectColorX - 15, selectColorY - 15, 30, 30);
                 myPen = new Pen(Color.White);
                 selectColor_G.DrawEllipse(myPen, selectColorX - 16, selectColorY - 16, 32, 32);


                 Color c = (SelectColor_BMP).GetPixel(selectColorX, selectColorY);
                 PixelDraw_Operate.Color_R = c.R;
                 PixelDraw_Operate.Color_G = c.G;
                 PixelDraw_Operate.Color_B = c.B;
             }
             catch { }*/
        }
    }
}

