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

using System.Runtime.InteropServices;

using AcadApp = Autodesk.AutoCAD.ApplicationServices.Application;

using MyXData.Core;

namespace MyXData.DockingPalette
{
    public partial class xdataForm : UserControl
    {
        //解决焦点问题(面板它会自动获取焦点，导致cad主窗口不能正常的获得焦点)
        [DllImport("user32.dll")]
        private static extern System.IntPtr SetFocus(System.IntPtr hwnd);


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
                if (xdata.HasXData())
                {
                    if (!xdata.IsXDataUpdate)
                    {
                        DialogResult res = MessageBox.Show("请确认是否保存数据?", "保存数据", MessageBoxButtons.YesNo);

                        if (res == DialogResult.Yes)
                        {
                            checkChanged();//检查修改，并更新
                            xdata.UpdateXData();

                            appNamesComboBox.Items.Clear();
                            xdataListView.Items.Clear();
                            appNamesComboBox.Text = null;
                        }
                    }
                }

                //xdata.UpdateXData();
            }

            //TestPalettes.SetPaletteFocus(false);
            SetFocus(Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Window.Handle); 

            PromptEntityOptions prEnt = new PromptEntityOptions("\n请选择一个实体 ");
            PromptEntityResult prEntRes = Tools.Editor.GetEntity(prEnt);
            if (prEntRes.Status != PromptStatus.OK)
            {
                return;
            }

            //TestPalettes.SetPaletteFocus(true);
            if (xdata == null || xdata.Id != prEntRes.ObjectId)
            {
                appNamesComboBox.Items.Clear();
                xdataListView.Items.Clear();
                appNamesComboBox.Text = null;

                xdata = new XData(prEntRes.ObjectId);
                updateData();
                MessageBox.Show("该实体没有扩展数据，请注册应用程序名称");
            }

            SetFocus(TestPalettes.ps.Handle);

        }

        //更新显示数据
        private void updateData()
        {
            if (xdata != null)
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

            Dictionary<string, string> ht = xdata.GetParamsWithAppName(appName);
            if (ht == null)
            {
                return;
            }

            ListViewItem item = new ListViewItem();

            foreach (KeyValuePair<string, string> de in ht)
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
            if (collection == null || collection.Count == 0)
            {
                return;
            }
            else
            {
                xdata.ClearXData(xdata.getCurrentAppName());
                Dictionary<string, string> dic = new Dictionary<string, string>();
                foreach (ListViewItem item in collection)
                {
                    dic.Add(item.SubItems[0].Text.Trim(), item.SubItems[1].Text.Trim());
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

                    int index = this.appNamesComboBox.SelectedIndex + 1;
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
                    //this.addButton.Focus();
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

            //this.eraseButton.Focus();
        }

        private void xdataListView_SelectedIndexChanged(object sender, EventArgs e)
        {            
            if (this.xdataListView.SelectedIndices.Count > 0)
            {
                int index = this.xdataListView.SelectedIndices[0];
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
                //else
                //{
                //    this.Close();
                //    return;
                //}
            }
            //this.Close();
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
                //else
                //{
                //    this.Close();
                //    return;
                //}
            }
            //this.Close();
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


        //拖拽自定义类
        /// <summary>
        /// Drop target to control the drop behavior to the AutoCAD window
        /// </summary>
        public class MyDropTarget : Autodesk.AutoCAD.Windows.DropTarget
        {
            public override void OnDragEnter(System.Windows.Forms.DragEventArgs e)
            {
                System.Diagnostics.Debug.WriteLine("DragEnter");
            }

            public override void OnDragLeave()
            {
                System.Diagnostics.Debug.WriteLine("DragLeave");
            }

            public override void OnDragOver(System.Windows.Forms.DragEventArgs e)
            {
                System.Diagnostics.Debug.WriteLine("DragOver");
            }
            static string data;
            public override void OnDrop(System.Windows.Forms.DragEventArgs e)
            {
                System.Diagnostics.Debug.WriteLine("Drop");
                //let's drop it
                //stash away the payload
                data = (string)e.Data.GetData(typeof(string));
                //start a command to handle the interaction with the user. Don't do it directly from the OnDrop method
                AcadApp.DocumentManager.MdiActiveDocument.SendStringToExecute("netdrop\n", false, false, false);
            }
        }


        //关于拖拽的一些事件处理
        private void TestControl_GiveFeedback(object sender, System.Windows.Forms.GiveFeedbackEventArgs e)
        {
            System.Diagnostics.Debug.WriteLine("GiveFeedback");
        }

        private void TestControl_QueryContinueDrag(object sender, System.Windows.Forms.QueryContinueDragEventArgs e)
        {
            System.Diagnostics.Debug.WriteLine("QueryContinueDrag");
        }

        private void label1_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            if (System.Windows.Forms.Control.MouseButtons == System.Windows.Forms.MouseButtons.Left)
            {
                //start dragDrop operation, MyDropTarget will be called when the cursor enters the AutoCAD view area.
                AcadApp.DoDragDrop(this, "Drag & drop successful!!!", System.Windows.Forms.DragDropEffects.All, new MyDropTarget());
            }
        }

        //下面的2个事件处理程序主要是对付面板焦点问题（可以编辑的控件如组合框、编辑框等不能获得焦点）
        //需要将面板ps的keepfocus属性设置为true
        //在这里使用静态函数进行处理（迫不得已的办法）
        private void appNamesComboBox_DropDown(object sender, EventArgs e)
        {
            TestPalettes.SetPaletteFocus(true);
        }

        private void appNamesComboBox_DropDownClosed(object sender, EventArgs e)
        {
            TestPalettes.SetPaletteFocus(false);
        }
    }
}
