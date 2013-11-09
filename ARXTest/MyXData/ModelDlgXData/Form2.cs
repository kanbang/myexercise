using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using MyXData.Core;

namespace MyXData.ModalDlg
{
    public partial class addNewXDataForm : Form
    {
        private string appName=null;
        private string param=null;
        private string value=null;
        private XData xdata=null;

        public string AppName
        {
            get 
            {
                return this.appName;
            }
            set
            {
                this.appName = value;
            }
        }

        public string Param
        {
            get
            {
                return this.param;
            }
            set
            {
                this.param = value;
            }
        }

        public string Value
        {
            get
            {
                return this.value;
            }
            set
            {
                this.value = value;
            }
        }


        public addNewXDataForm(XData xdata)
        {
            this.xdata = xdata;
            InitializeComponent();
        }


        private bool validateAppName()
        {
            bool isEffective = true;
            if (appName == null)
            {
                isEffective = false;
                MessageBox.Show("应用程序名不能为空!");
                this.appNameTextBox.Focus();
            }
            else if (appName.Length == 0)
            {
                isEffective = false;
                this.appNameTextBox.SelectAll();
                this.appNameTextBox.Focus();
                MessageBox.Show("请不要输入空白");
            }
            else if (xdata.HasXData())
            {
                ICollection appnames = xdata.GetAppNames();
                bool isExist = false;

                foreach (string app in appnames)
                {
                    if (appName == app)
                    {
                        isExist = true;
                        break;
                    }
                }
                if (isExist)
                {
                    isEffective = false;
                    MessageBox.Show("该扩展数据应用程序名称已经注册!");
                    this.appNameTextBox.Focus();
                }
            }

            return isEffective;
        }

        private bool validateParam()
        {
            bool isEffective = true;
            if (param == null)
            {
                isEffective = false;
                MessageBox.Show("必须设置参数名称!");
                this.paramTextBox.Focus();
            }
            else if (param.Length == 0)
            {
                isEffective = false;
                this.paramTextBox.SelectAll();
                this.paramTextBox.Focus();
                MessageBox.Show("请不要输入空白");
            }
            return isEffective;
        }

        private bool validateValue()
        {
            bool isEffective = true;
            if (value == null)
            {
                value = "null";
            }
            else if (value.Length == 0)
            {
                value = "null";
            }

            return isEffective;
        }

        private void obButton_Click(object sender, EventArgs e)
        {
            if (this.validateAppName() &&
                this.validateParam() &&
                this.validateValue())
            {
                this.Close();
            }
            
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            param = value = appName = null;
            this.Close();
        }

        private void appNameTextBox_TextChanged(object sender, EventArgs e)
        {
            this.appName = this.appNameTextBox.Text.Trim();
        }

        private void paramTextBox_TextChanged(object sender, EventArgs e)
        {
            this.param = this.paramTextBox.Text.Trim();
        }

        private void valueTextBox_TextChanged(object sender, EventArgs e)
        {
            this.value = this.valueTextBox.Text.Trim();
        }
    }
}
