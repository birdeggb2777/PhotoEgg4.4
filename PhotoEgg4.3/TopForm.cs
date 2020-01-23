using PhotoEgg4._3.Properties;
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
    public partial class TopForm : Form
    {
        Form1 form;
        public TopForm(Form1 f)
        {
            InitializeComponent();
            form = f;
        }

        private void TopForm_Load(object sender, EventArgs e)
        {
            pictureBox1.Image = Resources.Title;
            pictureBox2.Image = Resources.Title1;
            pictureBox1.Size = Resources.Title.Size;
            pictureBox2.Size = Resources.Title1.Size;
            pictureBox1.Location =new Point(this.Width - pictureBox1.Width,0);
            pictureBox2.Location=new Point(this.Width - pictureBox1.Width- pictureBox2.Width, 0);
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            form.Dispose();
        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            form.WindowState = FormWindowState.Minimized;
        }
    }
}
