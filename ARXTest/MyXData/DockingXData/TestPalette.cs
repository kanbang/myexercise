using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using AcadApp = Autodesk.AutoCAD.ApplicationServices.Application;

namespace MyXData.DockingPalette
{
    public class TestPalettes
    {
        public static Autodesk.AutoCAD.Windows.PaletteSet ps;

        public static void SetPaletteFocus(bool isFocus)
        {
            ps.KeepFocus = isFocus;
        }

        [Autodesk.AutoCAD.Runtime.CommandMethod("dockingxdata")]
        public static void DoIt()
        {
            if (ps == null)
            {
                //use constructor with Guid so that we can save/load user data
                ps = new Autodesk.AutoCAD.Windows.PaletteSet("Test Palette Set", new Guid("{00C0E891-C266-43e4-B95C-613255855BE1}"));
                ps.Load += new Autodesk.AutoCAD.Windows.PalettePersistEventHandler(ps_Load);
                ps.Save += new Autodesk.AutoCAD.Windows.PalettePersistEventHandler(ps_Save);
                ps.Style = Autodesk.AutoCAD.Windows.PaletteSetStyles.NameEditable |
                    Autodesk.AutoCAD.Windows.PaletteSetStyles.ShowPropertiesMenu |
                    Autodesk.AutoCAD.Windows.PaletteSetStyles.ShowAutoHideButton |
                    Autodesk.AutoCAD.Windows.PaletteSetStyles.ShowCloseButton;
                ps.MinimumSize = new System.Drawing.Size(350, 300);
                ps.Add("XData Palette", new xdataForm(null));
            }
            bool b = ps.Visible;

            ps.Dock = Autodesk.AutoCAD.Windows.DockSides.None;
            ps.Visible = true;

            ps.Dock = Autodesk.AutoCAD.Windows.DockSides.Left;
            Autodesk.AutoCAD.EditorInput.Editor e = AcadApp.DocumentManager.MdiActiveDocument.Editor;


            //下面的主要是设置palette的一些相关属性的
            //在后面的开发中可以做一些相应的简化
            //例如，直接设置一些值
            //Autodesk.AutoCAD.EditorInput.PromptResult res = e.GetKeywords("Select a palette set option:", "Opacity", "TitleBarLocation", "Docking");
            //if (res.Status == Autodesk.AutoCAD.EditorInput.PromptStatus.OK)
            //{
            //    switch (res.StringResult)
            //    {
            //        case "Opacity":
            //            SetOpacity();
            //            break;

            //        case "TitleBarLocation":
            //            SetTitleBarLocation();
            //            break;

            //        case "Docking":
            //            SetDocking();
            //            break;                        
            //    }
            //}
        }

        private static void ps_Load(object sender, Autodesk.AutoCAD.Windows.PalettePersistEventArgs e)
        {
            //demo loading user data
            double a = (double)e.ConfigurationSection.ReadProperty("whatever", 22.3);
        }

        private static void ps_Save(object sender, Autodesk.AutoCAD.Windows.PalettePersistEventArgs e)
        {
            //demo saving user data
            e.ConfigurationSection.WriteProperty("whatever", 32.3);
        }


       static  private void  SetOpacity()
        {
            Autodesk.AutoCAD.EditorInput.Editor e = AcadApp.DocumentManager.MdiActiveDocument.Editor;

            Autodesk.AutoCAD.EditorInput.PromptIntegerResult resInt;
            do
            {
                resInt = e.GetInteger("Enter opacity:");
                if (resInt.Status != Autodesk.AutoCAD.EditorInput.PromptStatus.OK)
                    break;
                if (resInt.Value >= 0 && resInt.Value <= 100)
                    break;
                e.WriteMessage("Opacity must be between 0 and 100\n");
            }
            while (true);

            ps.Opacity = resInt.Value;
        }

        static private void  SetTitleBarLocation()
        {
            Autodesk.AutoCAD.EditorInput.Editor e = AcadApp.DocumentManager.MdiActiveDocument.Editor;
            Autodesk.AutoCAD.EditorInput.PromptResult res;

            res = e.GetKeywords("Select titlebar location:", "Left", "Right");
            if (res.Status == Autodesk.AutoCAD.EditorInput.PromptStatus.OK)
                switch (res.StringResult)
                {
                    case "Left":
                        ps.TitleBarLocation = Autodesk.AutoCAD.Windows.PaletteSetTitleBarLocation.Left;
                        break;
                    case "Right":
                        ps.TitleBarLocation = Autodesk.AutoCAD.Windows.PaletteSetTitleBarLocation.Right;
                        break;
                }            
        }

        static private void SetDocking()
        {
            Autodesk.AutoCAD.EditorInput.Editor e = AcadApp.DocumentManager.MdiActiveDocument.Editor;
            Autodesk.AutoCAD.EditorInput.PromptResult res;

            res = e.GetKeywords("Choose a docking option:", "None", "Left", "Right", "Top", "Bottom");
            if (res.Status == Autodesk.AutoCAD.EditorInput.PromptStatus.OK)
            {
                switch (res.StringResult)
                {
                    case "None":
                        ps.Dock = Autodesk.AutoCAD.Windows.DockSides.None;
                        break;
                    case "Left":
                        ps.Dock = Autodesk.AutoCAD.Windows.DockSides.Left;
                        break;
                    case "Right":
                        ps.Dock = Autodesk.AutoCAD.Windows.DockSides.Right;
                        break;
                    case "Top":
                        ps.Dock = Autodesk.AutoCAD.Windows.DockSides.Top;
                        break;
                    case "Bottom":
                        ps.Dock = Autodesk.AutoCAD.Windows.DockSides.Bottom;
                        break;
                }
            }
        }
    }
}
