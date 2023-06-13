using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using  FlashLib;
namespace WindowsFormsTest
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            this.Load += Form1_Load;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            FlashHelper = new FlashWraper();
            FlashHelper.Init(this.Handle);
            FlashHelper.LoadFlash($"{Environment.CurrentDirectory}\\main.swf");
            FlashHelper.FlashMessage += FlashHelper_FlashMessage;
        }

        private void FlashHelper_FlashMessage(string msg)
        {
       
            MessageBox.Show(msg);
        }

        FlashWraper FlashHelper;
    }
}
