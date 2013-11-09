using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Collections.Specialized;

namespace MyXData.DockingPalette
{
    public partial class Form4 : Form
    {
        public Form4(StringCollection appnames)
        {
            InitializeComponent();            

            if (appnames == null || appnames.Count == 0)
            {
                appName = null;
                isDelAllXData = false;
                delAllXDatCheckBox.Enabled = false;//如果没有数据禁用
            }
            else
            {
                this.appnames = appnames;
                foreach (string str in appnames)
                {
                    appNamesComboBox.Items.Add(str);
                }
                appNamesComboBox.SelectedIndex = 0;//默认选择第一个
            }
        }

        public bool IsDelAllXData
        {
            get 
            {
                return this.isDelAllXData;
            }            
        }

        public string DelAppName
        {
            get            
            {
                return this.appName;
            }
        }
        private bool isDelAllXData = false;
        private string appName = null;
        private StringCollection appnames = null;

        private void obButton_Click(object sender, EventArgs e)
        {
            appName = appNamesComboBox.SelectedItem.ToString();
            isDelAllXData = delAllXDatCheckBox.Checked;
            this.Close();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            appName = null;
            isDelAllXData = false;
            this.Close();
        }

        private void delAllXDatCheckBox_Click(object sender, EventArgs e)
        {
            if (delAllXDatCheckBox.Checked)
            {
                appNamesComboBox.Enabled = false;
            }
            else
            {
                appNamesComboBox.Enabled = true;
            }
        }
    }
}
