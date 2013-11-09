using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using MyXData.Core;
namespace MyXData.ModalDlg
{
    public partial class AddParamsForm : Form
    {
        private string param = null;
        private string value = null;
        private XData xdata = null;
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

        public bool validateParam()
        {
            bool isEffective = true;
            if (this.param == null)
            {
                isEffective = false;
                MessageBox.Show("参数名不能为空");
                this.paramTextBox.Focus();
            }
            else if (param.Length == 0)
            {
                isEffective = false;
                this.paramTextBox.SelectAll();
                this.paramTextBox.Focus();
                MessageBox.Show("请不要输入空白");
            }
            else if (xdata.GetAppNames().Count==0)
            {
                isEffective=false;
                MessageBox.Show("该实体没有扩展数据，请先注册应用程序名!");
                this.param = null;
                this.value = null;
                this.Close();
            }
            else
            {
                Dictionary<string, string> ht = xdata.GetParamsWithAppName(xdata.getCurrentAppName());
                if (ht.ContainsKey(this.param))
                {
                    isEffective = false;
                    MessageBox.Show("该参数名已经存在，请不要重复使用一个名称!");
                    this.paramTextBox.Focus();
                }
                else
                {
                    //do nothing
                }

            }

            return isEffective;
        }

        public bool validateValue()
        {
            bool isEffective = true;
            if (this.value == null)//允许参数值为null
            {
                this.value = "null";
            }
            else if (value.Length == 0)
            {
                this.value = "null";
            }

            return isEffective;
        }

        public AddParamsForm(XData xdata)//必须保证xdata!=null(在外部保证)
        {
            this.xdata = xdata;
            InitializeComponent();
        }

        private void paramTextBox_TextChanged(object sender, EventArgs e)
        {
            this.param = this.paramTextBox.Text.Trim();
        }

        private void valueTextBox_TextChanged(object sender, EventArgs e)
        {
            this.value = this.valueTextBox.Text.Trim();
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            if (this.validateParam() && this.validateValue())
            {
                this.Close();
            }
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            this.param = null;
            this.value = null;
            this.Close();
        }
    }
}
