using System;
using System.Linq;
using System.Text;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.Runtime;

using DNA;
using CustomListView;//可编辑的listview

using MyXData.Core;

namespace MyXData.ModalDlg
{
    /// <summary>
    /// Summary for TestXData.
    /// </summary>
    public class TestXData
    {

        [CommandMethod("ModalXData", CommandFlags.UsePickSet)]
        public void Test()
        {
            //命令前选择 pick-first技术
            //在执行命令前，已经存在了选择集
            //一般都是用selectImplied()方法
            //这里用selectPrevious也能达到目的
            Editor ed = Tools.Editor;
            //PromptSelectionResult sr = ed.SelectPrevious();
            PromptSelectionResult sr = ed.SelectImplied();
            if (sr.Status == PromptStatus.OK)
            {
                SelectionSet ss = sr.Value;
                if (ss == null || ss.Count == 0)
                {
                    xdataForm modalForm = new xdataForm(null);
                    Autodesk.AutoCAD.ApplicationServices.Application.ShowModalDialog(modalForm);
                }
                else
                {
                    xdataForm modalForm = new xdataForm(new XData(ss[0].ObjectId));
                    Autodesk.AutoCAD.ApplicationServices.Application.ShowModalDialog(modalForm);
                }
            }
            else
            {
                xdataForm modalForm = new xdataForm(null);
                Autodesk.AutoCAD.ApplicationServices.Application.ShowModalDialog(modalForm);
            }
        }
    }
}