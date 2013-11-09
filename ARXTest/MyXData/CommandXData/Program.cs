using System;
//using System.Linq;
//using System.Text;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.Runtime;

using DNA;

//using System.Reflection;
using System.Collections.Specialized;


using MyXData.Core;

namespace MyXData.Command
{
    public class TestXData
    {
        //[CommandMethod("TC")]

        //static public void ListCommandsFromThisAssembly()
        //{
        //    // Just get the commands for this assembly

        //    DocumentCollection dm = Application.DocumentManager;

        //    Editor ed = dm.MdiActiveDocument.Editor;

        //    Assembly asm = Assembly.GetExecutingAssembly();
        //    string[] cmds = GetCommands(asm, false);
        //    ed.WriteMessage("\n自定义命令列表：\n");
        //    foreach (string cmd in cmds)
        //    {
        //        ed.WriteMessage(cmd + "\n");
        //    }
        //}

        //[CommandMethod("LCM")]
        //static public void ListMarkedCommands()
        //{

        //    // Get the commands for all assemblies,
        //    //  but only those with explicit
        //    // CommandClass attributes (much quicker)

        //    StringCollection cmds = new StringCollection();

        //    DocumentCollection dm = Application.DocumentManager;

        //    Editor ed = dm.MdiActiveDocument.Editor;

        //    Assembly[] asms = AppDomain.CurrentDomain.GetAssemblies();
        //    foreach (Assembly asm in asms)
        //    {
        //        cmds.AddRange(GetCommands(asm, true));
        //    }

        //    ed.WriteMessage("\n自定义命令列表：\n");
        //    foreach (string cmd in cmds)
        //    {
        //        ed.WriteMessage(cmd + "\n");
        //    }
        //}


        //[CommandMethod("LC")]
        //static public void ListCommands()
        //{

        //    // Get the commands for all assemblies,
        //    // marked or otherwise (much slower)

        //    StringCollection cmds = new StringCollection();
        //    DocumentCollection dm = Application.DocumentManager;
        //    Editor ed = dm.MdiActiveDocument.Editor;

        //    Assembly[] asms = AppDomain.CurrentDomain.GetAssemblies();
        //    foreach (Assembly asm in asms)
        //    {
        //        cmds.AddRange(GetCommands(asm, false));
        //    }

        //    ed.WriteMessage("\n自定义命令列表：\n");
        //    foreach (string cmd in cmds)
        //    {
        //        ed.WriteMessage(cmd + "\n");
        //    }
        //}


        //private static string[] GetCommands(Assembly asm, bool markedOnly)
        //{

        //    StringCollection sc = new StringCollection();

        //    object[] objs = asm.GetCustomAttributes(typeof(CommandClassAttribute), true);

        //    Type[] tps;

        //    int numTypes = objs.Length;

        //    if (numTypes > 0)
        //    {
        //        tps = new Type[numTypes];
        //        for (int i = 0; i < numTypes; i++)
        //        {
        //            CommandClassAttribute cca = objs[i] as CommandClassAttribute;

        //            if (cca != null)
        //            {
        //                tps[i] = cca.Type;
        //            }
        //        }
        //    }
        //    else
        //    {
        //        // If we're only looking for specifically
        //        // marked CommandClasses, then use an
        //        // empty list

        //        if (markedOnly)
        //            tps = new Type[0];
        //        else
        //            tps = asm.GetExportedTypes();
        //    }

        //    foreach (Type tp in tps)
        //    {
        //        MethodInfo[] meths = tp.GetMethods();
        //        foreach (MethodInfo meth in meths)
        //        {
        //            objs = meth.GetCustomAttributes(
        //                typeof(CommandMethodAttribute),
        //                true
        //              );

        //            foreach (object obj in objs)
        //            {
        //                CommandMethodAttribute attb = (CommandMethodAttribute)obj;
        //                sc.Add(attb.GlobalName);
        //            }
        //        }
        //    }

        //    string[] ret = new string[sc.Count];
        //    sc.CopyTo(ret, 0);
        //    return ret;
        //}

        //[CommandMethod("Test", CommandFlags.UsePickSet)]
        //public void Test()
        //{
        //    ////命令前选择 pick-first技术
        //    ////在执行命令前，已经存在了选择集
        //    ////一般都是用selectImplied()方法
        //    ////这里用selectPrevious也能达到目的
        //    //Editor ed = Tools.Editor;
        //    ////PromptSelectionResult sr = ed.SelectPrevious();
        //    //PromptSelectionResult sr = ed.SelectImplied();
        //    //if (sr.Status == PromptStatus.OK)
        //    //{
        //    //    SelectionSet ss = sr.Value;
        //    //    if (ss == null || ss.Count == 0)
        //    //    {
        //    //        xdataForm modalForm = new xdataForm(null);
        //    //        Autodesk.AutoCAD.ApplicationServices.Application.ShowModalDialog(modalForm);
        //    //    }
        //    //    else
        //    //    {
        //    //        xdataForm modalForm = new xdataForm(new XData(ss[0].ObjectId));
        //    //        Autodesk.AutoCAD.ApplicationServices.Application.ShowModalDialog(modalForm);
        //    //    }
        //    //}
        //    //else
        //    //{
        //    //    xdataForm modalForm = new xdataForm(null);
        //    //    Autodesk.AutoCAD.ApplicationServices.Application.ShowModalDialog(modalForm);
        //    //}
        //}

        [CommandMethod("printxdata")]
        static public void printxdata()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            ObjectId id = SelectEntity();//选择实体

            if (id == ObjectId.Null)
            {
                ed.WriteMessage("\n用户取消操作\n");
                return;
            }
            XData xdata = new XData(id);
            if (!xdata.HasXData())
            {
                Tools.WriteMessage("\n该实体不存在扩展数据\n");
                return;
            }

            bool isPrintAllXData = AskForPrintAllXData();
            if (isPrintAllXData)
            {
                xdata.PrintXDataList();
            }
            else
            {
                string appName = getAppName();//获取appname
                if (appName == null)
                {
                    ed.WriteMessage("\n用户取消操作\n");
                    return;
                }
                xdata.PrintXData(appName);
            }
        }

        [CommandMethod("addxdata")]
        static public void addxdata()
        {
            bool isCreateNewXdata = false;
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            ObjectId id = SelectEntity();//选择实体

            if (id == ObjectId.Null)
            {
                ed.WriteMessage("\n用户取消操作\n");
                return;
            }
            XData xdata = new XData(id);
            if (!xdata.HasXData())
            {
                Tools.WriteMessage("\n该实体不存在扩展数据\n");
                isCreateNewXdata = AskForCreateXData();
                if (!isCreateNewXdata)
                {
                    Tools.WriteMessage("\n用户选择不创建新的扩展数据，程序结束\n");
                    return;
                }
                Tools.WriteMessage("\n下面的操作将会创建新的扩展数据\n");
            }

            string appName = getAppName();//获取appname
            if (appName == null)
            {
                ed.WriteMessage("\n用户取消操作\n");
                return;
            }

            string param = null;
            string value = null;
            bool isUserCancel = false;


            if (!XData.IsAppExist(appName))
            {
                Tools.WriteMessage("\n该应用程序名对应的扩展数据不存在\n");
                Tools.WriteMessage("\n下面的操作将会注册该应用程序名称，并添加一个新的扩展数据\n");
                XData.RegisterApp(appName);
            }
            createNewXData(appName, ref xdata);//不管appname存在与否，调用cretaenewxdata都不会产生任何的问题         

            xdata.setCurrentAppName(appName);//设置当前appname

            //重复循环，让用户输入扩展数据的参数和值
            Tools.WriteMessage("\n继续添加数据\n");
            isUserCancel = true;
            param = null;
            value = null;
            while (isUserCancel)
            {
                isUserCancel = getParams(ref param, ref value);
                if (param != null && value != null)
                {
                    xdata.AddXData(param, value);//添加扩展数据，成对添加                
                }
            }

            xdata.UpdateXData();
        }

        [CommandMethod("delallxdata")]
        static public void delallxdata()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            ObjectId id = SelectEntity();//选择实体
            if (id == ObjectId.Null)
            {
                ed.WriteMessage("\n用户取消操作\n");
                return;
            }
            XData xdata = new XData(id);
            if (!xdata.HasXData())
            {
                Tools.WriteMessage("\n该实体不存在扩展数据\n");
                return;
            }

            xdata.ClearAllXData();
            xdata.UpdateXData();
        }

        [CommandMethod("delxdata")]
        static public void delxdata()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            ObjectId id = SelectEntity();//选择实体
            if (id == ObjectId.Null)
            {
                ed.WriteMessage("\n用户取消操作\n");
                return;
            }
            XData xdata = new XData(id);
            if (!xdata.HasXData())
            {
                Tools.WriteMessage("\n该实体不存在扩展数据\n");
                return;
            }

            string appName = getAppName();//获取appname
            if (appName == null)
            {
                ed.WriteMessage("\n用户取消操作\n");
                return;
            }

            if (!XData.IsAppExist(appName))
            {
                Tools.WriteMessage("\n该应用程序名对应的扩展数据不存在\n");
                return;
            }

            xdata.ClearXData(appName);
            xdata.UpdateXData();
        }

        [CommandMethod("delxdataparam")]
        static public void delxdataparam()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            ObjectId id = SelectEntity();//选择实体
            if (id == ObjectId.Null)
            {
                ed.WriteMessage("\n用户取消操作\n");
                return;
            }
            XData xdata = new XData(id);
            if (!xdata.HasXData())
            {
                Tools.WriteMessage("\n该实体不存在扩展数据\n");
                return;
            }

            string appName = getAppName();//获取appname
            if (appName == null)
            {
                ed.WriteMessage("\n用户取消操作\n");
                return;
            }

            if (!XData.IsAppExist(appName))
            {
                Tools.WriteMessage("\n该应用程序名对应的扩展数据不存在\n");
                return;
            }

            string param = getParam("\n请输入一个参数名称：");
            if (param == null)
            {
                Tools.WriteMessage("\n用户取消操作\n");
                return;
            }

            xdata.DeleteXData(param);
            xdata.UpdateXData();
        }

        [CommandMethod("getxdata")]
        static public void getxdata()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            ObjectId id = SelectEntity();//选择实体
            if (id == ObjectId.Null)
            {
                ed.WriteMessage("\n用户取消操作\n");
                return;
            }
            XData xdata = new XData(id);
            if (!xdata.HasXData())
            {
                Tools.WriteMessage("\n该实体不存在扩展数据\n");
                return;
            }

            string appName = getAppName();//获取appname
            if (appName == null)
            {
                ed.WriteMessage("\n用户取消操作\n");
                return;
            }

            if (!XData.IsAppExist(appName))
            {
                Tools.WriteMessage("\n该应用程序名对应的扩展数据不存在\n");
                return;
            }

            xdata.PrintXData(appName);

        }

        static public ObjectId SelectEntity()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            PromptEntityOptions pEntity = new PromptEntityOptions("\n请选择一个实体 ");
            PromptEntityResult pEntityRes = ed.GetEntity(pEntity);

            if (pEntityRes.Status == PromptStatus.OK)
            {

                return pEntityRes.ObjectId;
            }
            else
            {
                return ObjectId.Null;
            }
        }

        static public bool AskForCreateXData()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            PromptKeywordOptions pKey = new PromptKeywordOptions("\n是否要创建新的扩展数据 ");
            pKey.AllowNone = true;
            pKey.Keywords.Add("Yes");
            pKey.Keywords.Add("No");
            pKey.Keywords.Default = "No";

            PromptResult pKeyRes = ed.GetKeywords(pKey);

            if (pKeyRes.StringResult == "Yes")
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        static public bool AskForPrintAllXData()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            PromptKeywordOptions pKey = new PromptKeywordOptions("\n是否要打印所有的扩展数据 ");
            pKey.AllowNone = true;
            pKey.Keywords.Add("Yes");
            pKey.Keywords.Add("No");
            pKey.Keywords.Default = "Yes";

            PromptResult pKeyRes = ed.GetKeywords(pKey);

            if (pKeyRes.StringResult == "Yes")
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        static public string getParam(string message)
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            PromptStringOptions pStr = null;
            PromptResult pStrRes = null;
            while (true)
            {
                pStr = new PromptStringOptions(message);
                pStr.AllowSpaces = false;
                pStrRes = ed.GetString(pStr);
                if (pStrRes.Status == PromptStatus.OK)
                {
                    if (pStrRes.StringResult.Length == 0)
                    {
                        continue;
                    }
                    else
                    {
                        return pStrRes.StringResult;
                    }
                }
                else
                {
                    return null;
                }
            }
        }


        static public bool getParams(ref string param, ref string value)
        {
            param = null;
            value = null;

            bool isUserCancel = false;

            while (!isUserCancel)
            {
                param = getParam("\n请输入参数名 ");
                if (param != null)
                {
                    isUserCancel = true;
                }
                else
                {
                    param = null;
                    value = null;
                    return false;
                }
            }

            isUserCancel = false;

            while (!isUserCancel)
            {
                value = getParam("\n请输入参数值 ");
                if (value != null)
                {
                    isUserCancel = true;
                }
                else
                {
                    param = null;
                    value = null;
                    return false;
                }
            }

            return true;
        }

        public static string getAppName()
        {
            return getParam("\n输入应用程序名称 ");
        }

        static public void createNewXData(string appName, ref XData xdata)
        {
            string param = null;
            string value = null;
            bool isUserCancel = false;

            Tools.WriteMessage("\n创建新的扩展数据!!!至少需要2个数据项\n");
            isUserCancel = getParams(ref param, ref value);

            if (!isUserCancel)
            {
                Tools.WriteMessage("\n用户取消操作\n");
                return;
            }
            xdata.CreateXData(appName, param, value);//创建一个新的扩展数据

        }
    }
}
