using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Collections.Specialized;

using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.Runtime;
using DNA;

using MyXData.Core;

//using System.Runtime.InteropServices;

namespace MyXData.ModalDlg
{
    public partial class xdataForm : Form
    {
        ////解决焦点问题(面板它会自动获取焦点，导致cad主窗口不能正常的获得焦点)
        //[DllImport("user32.dll")]
        //private static extern System.IntPtr SetFocus(System.IntPtr hwnd);

        private XData xdata = null;

        public xdataForm(XData xd)
        {
            InitializeComponent();
            this.xdataListView.AddEditableCell(-1, 0);
            this.xdataListView.AddEditableCell(-1, 1);
            this.xdataListView.AddSubItem = true;

            xdata = xd;

            if (xdata != null)
            {
                updateData();
            }
        }

        private void selectButton_Click(object sender, EventArgs e)
        {
            if (xdata != null)
            {
                xdata.UpdateXData();
            }

            //SetFocus(Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Window.Handle); 

            this.Visible = false;
            this.Hide();//先隐藏窗体

            PromptEntityOptions prEnt = new PromptEntityOptions("\n请选择一个实体 ");
            PromptEntityResult prEntRes = Tools.Editor.GetEntity(prEnt);
            if (prEntRes.Status != PromptStatus.OK)
            {
                this.Visible = true;
                this.Show();
                return;
            }

            if (xdata == null || xdata.Id != prEntRes.ObjectId)
            {
                xdata = new XData(prEntRes.ObjectId);
                updateData();
            }

            this.Visible = true;
            this.Show();

            //SetFocus(this.Handle);

        }

        //更新显示数据
        private void updateData()
        {
            if(xdata!=null)
            {
                this.appNamesComboBox.Items.Clear();
                this.xdataListView.Items.Clear();
                
                if (!xdata.HasXData())
                {
                    return;
                }
                else
                {
                    IEnumerator appname = xdata.GetAppNames().GetEnumerator();
                    while (appname.MoveNext())
                    {
                        this.appNamesComboBox.Items.Add(appname.Current);
                    }
                }

                if (xdata.GetAppNames().Count != 0)
                {
                    this.appNamesComboBox.SelectedIndex = 0;//触发SelectedIndexChanged事件，从而填充listview                    
                }
            }
        }

        private void fiilListView(string appName)//填充当前的appname对应的参数和值
        {
            this.xdataListView.Items.Clear();

            Dictionary<string,string> ht = xdata.GetParamsWithAppName(appName);
            if (ht == null)
            {
                return;
            }

            ListViewItem item = new ListViewItem();

            foreach (KeyValuePair<string,string> de in ht)
            {
                item.SubItems[0].Text = de.Key;
                item.SubItems.Add(de.Value);
                this.xdataListView.Items.Add(item);
                item = new ListViewItem();
            }
        }

        private void checkChanged()//更新数据
        {
            ListView.ListViewItemCollection collection = xdataListView.Items;
            if (collection ==null  || collection.Count == 0)
            {
                return;
            }
            else
            {
                xdata.ClearXData(xdata.getCurrentAppName());
                Dictionary<string, string> dic = new Dictionary<string, string>();
                foreach (ListViewItem item in collection)
                {
                    dic.Add(item.SubItems[0].Text.Trim(),item.SubItems[1].Text.Trim());
                }

                xdata.CreateXData(xdata.getCurrentAppName(), dic);
            }
        }

        private void appNamesComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            checkChanged();
            string appName = this.appNamesComboBox.SelectedItem.ToString();
            xdata.setCurrentAppName(appName);//设置当前appname
            this.fiilListView(appName);
        }

        private void newAppButton_Click(object sender, EventArgs e)
        {
 
            if (xdata != null)
            {
                addNewXDataForm addNewXDataForm = new addNewXDataForm(xdata);
                Autodesk.AutoCAD.ApplicationServices.Application.ShowModalDialog(addNewXDataForm);

                if (addNewXDataForm.AppName == null ||
                    addNewXDataForm.Param == null ||
                    addNewXDataForm.Value == null)
                {
                    //do nothing
                }
                else
                {
                    string appname = addNewXDataForm.AppName;
                    if (!XData.IsAppExist(appname))
                    {
                        XData.RegisterApp(addNewXDataForm.AppName);
                    }

                    xdata.CreateXData(appname, addNewXDataForm.Param, addNewXDataForm.Value);//内部

                    //xdata.setCurrentAppName(appname);//设置当前appname

                    int index = this.appNamesComboBox.SelectedIndex+1;
                    this.appNamesComboBox.Items.Insert(index, appname);//在原有的数据后面加入
                    this.appNamesComboBox.SelectedIndex = index;
                }
            }
            else
            {
                MessageBox.Show("必须选择一个实体!");
                this.selectButton.Focus();
            }
        }

        private void addButton_Click(object sender, EventArgs e)
        {           
            if (xdata != null)
            {
                if (!xdata.HasXData())
                {
                    MessageBox.Show("该实体没有扩展数据，请注册应用程序名");
                    newAppButton.Focus();
                    return;
                }

                AddParamsForm addParamsForm = new AddParamsForm(xdata);
                Autodesk.AutoCAD.ApplicationServices.Application.ShowModalDialog(addParamsForm);

                if (addParamsForm.Param == null || addParamsForm.Value == null)
                {
                    // do nothing
                }
                else
                {
                    xdata.AddXData(addParamsForm.Param, addParamsForm.Value);                   

                    ListViewItem item = new ListViewItem();
                    item.SubItems[0].Text = addParamsForm.Param;
                    item.SubItems.Add(addParamsForm.Value);

                    if (this.xdataListView.SelectedItems.Count > 0)
                    {
                        int index = this.xdataListView.SelectedItems[0].Index + 1;
                        this.xdataListView.Items.Insert(index, item);
                    }
                    else
                    {
                        this.xdataListView.Items.Add(item);
                    }
                }
            }
            else
            {
                MessageBox.Show("必须选择一个实体");
                this.selectButton.Focus();
            }
        }

        private void eraseButton_Click(object sender, EventArgs e)
        {
            if (xdata == null)
            {
                MessageBox.Show("必须选择一个实体");
                this.selectButton.Focus();
                return;
            }

            if (!xdata.HasXData())
            {
                MessageBox.Show("该实体没有扩展数据，请注册应用程序名");
                newAppButton.Focus();
                return;
            }


            if (this.xdataListView.SelectedItems.Count > 0)
            {
                if (this.xdataListView.Items.Count > 1)
                {
                    int index = this.xdataListView.SelectedItems[0].Index;
                    this.xdataListView.Items.RemoveAt(index);//删除指定的项

                    //设置选择项
                    if (index == this.xdataListView.Items.Count)
                    {
                        this.xdataListView.SelectedIndices.Add(index - 1);
                    }
                    else
                    {
                        this.xdataListView.SelectedIndices.Add(index);
                    }
                }
                else
                {
                    MessageBox.Show("至少保留一项数据!");
                    this.xdataListView.SelectedIndices.Add(0);//激活SelectedIndexChanged事件
                }
            }
            else
            {
                MessageBox.Show("必须选择一项数据，才能进行删除操作");
            }
        }

        private void xdataListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.xdataListView.SelectedIndices.Count > 0)
            {
                int index=this.xdataListView.SelectedIndices[0];
                this.xdataListView.Items[index].Selected = true;
                this.xdataListView.Focus();//只有当listview出于focus状态时，才能看到高亮显示
            }
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            if (xdata != null)
            {
                if (xdata.HasXData())
                {
                    checkChanged();//检查修改，并更新
                    xdata.UpdateXData();
                }
                else
                {
                    this.Close();
                    return;
                }
            }
            this.Close();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            if (xdata != null)
            {
                if (xdata.HasXData())
                {
                    DialogResult res = MessageBox.Show("请确认是否保存数据?", "保存数据", MessageBoxButtons.YesNo);

                    if (res == DialogResult.Yes)
                    {
                        checkChanged();//检查修改，并更新
                        xdata.UpdateXData();
                    }
                }
                else
                {
                    this.Close();
                    return;
                }
            }
            this.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (xdata == null)
            {
                MessageBox.Show("必须选择一个实体");
                this.selectButton.Focus();
                return;
            }

            if (!xdata.HasXData())
            {
                MessageBox.Show("该实体没有扩展数据，请注册应用程序名");
                newAppButton.Focus();
                return;
            }

            if (appNamesComboBox.Items.Count == 0)
            {
                MessageBox.Show("没有注册应用程序名存在，请注册应用程序名称");
                newAppButton.Focus();
            }
            else
            {
                StringCollection appnames = new StringCollection();
                foreach (Object obj in appNamesComboBox.Items)
                {
                    appnames.Add(obj as string);
                }
                Form4 form = new Form4(appnames);
                Autodesk.AutoCAD.ApplicationServices.Application.ShowModalDialog(form);

                string appName = form.DelAppName;
                bool isDelAllXData = form.IsDelAllXData;

                if (isDelAllXData)
                {
                    if (appName == null)
                    {
                        return;//do nothing
                    }
                    else
                    {
                        appNamesComboBox.Items.Clear();
                        xdataListView.Items.Clear();
                        xdata.ClearAllXData();
                    }
                }
                else
                {
                    if (appName == null)
                    {
                        return;//do nothing
                    }
                    else
                    {
                        if (appName == appNamesComboBox.SelectedItem.ToString())
                        {
                            MessageBox.Show("不能删除当前AppName");
                            return;
                        }
                        else
                        {
                            appNamesComboBox.Items.Remove(appName);
                            xdata.ClearXData(appName);
                        }
                    }
                }
            }
        }
    }
}
