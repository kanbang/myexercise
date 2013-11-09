using System;
using System.Windows.Forms;
using System.IO;
using Microsoft.Win32;

namespace GraphvizDraw
{
    public partial class GvDotGenerateForm : Form
    {
        public GvDotGenerateForm()
        {
            InitializeComponent();
            
            // 读取graphviz的安装目录
            gvDirTextBox.Text = readGraphvizInstallPath();

            // 向图形格式中添加数据
            initGraphFormatComBox();
        }

        private void initGraphFormatComBox()
        {
            gFormatCombox.Items.Add("png");
            gFormatCombox.Items.Add("ps");
            gFormatCombox.SelectedIndex = 0;
        }

        private string readGraphvizInstallPath()
        {
            RegistryKey hkml = Registry.LocalMachine;
            RegistryKey software = hkml.OpenSubKey("SOFTWARE");
            if (software == null)
            {
                return "";
            }

            RegistryKey attDir = software.OpenSubKey("AT&T Research Labs");
            if (attDir == null)
            {
                return "";
            }

            RegistryKey gvDir = attDir.OpenSubKey("Graphviz");
            if (gvDir == null)
            {
                return "";
            }
            return gvDir.GetValue("InstallPath").ToString();
        }

        private void selGvDirBtn_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog path = new FolderBrowserDialog();
            path.ShowDialog();
            this.gvDirTextBox.Text = path.SelectedPath;
        }

        private void selGPDataBtn_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "文本文件|*.txt";
            ofd.Title = "请选择一个通风网络拓扑数据文件";
            ofd.InitialDirectory = Application.StartupPath;

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                gpDataTextBox.Text = ofd.FileName;
            }
        }

        private void selSaveDirBtn_Click(object sender, EventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = string.Format("图形文件|*.{0}", gFormatCombox.SelectedItem.ToString());
            sfd.InitialDirectory = Application.StartupPath;
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                saveTextBox.Text = sfd.FileName;
            }
        }

        private bool checkData()
        {
            if (gvDirTextBox.Text.Length == 0)
            {
                MessageBox.Show("请选择Graphviz的安装目录\n例如: D:\\Program\\Graphviz2.26.3");
                return false;
            }
            else
            {
                if (!File.Exists(String.Format("{0}bin\\dot.exe", gvDirTextBox.Text)))
                {
                    MessageBox.Show("dot.exe不存在，指定Graphviz的安装路径是错误的!");
                    return false;
                }
            }

            if (gpDataTextBox.Text.Length == 0)
            {
                MessageBox.Show("请选择一个通风网络拓扑数据txt文件!");
                return false;
            }

            if (saveTextBox.Text.Length == 0 || !Path.HasExtension(saveTextBox.Text))
            {
                MessageBox.Show("请输入或选择一个有效的图形存储路径!");
                saveTextBox.Clear();
                return false;
            }
            else
            {
                string pathExt = Path.GetExtension(saveTextBox.Text).Substring(1);
                string selExt = gFormatCombox.SelectedItem.ToString();
                if(pathExt.CompareTo(selExt) != 0)
                {
                    MessageBox.Show(String.Format("输出图形路径的扩展名: {0}\n与\n指定图形格式: {1}不相同!", pathExt, selExt));
                    return false;
                }
            }
            return true;
        }

        private void genDotFile(string graphTopologyFile, string gvFile, bool hasTag)
        {
            FileStream inFileStream = new FileStream(graphTopologyFile, FileMode.Open);
            FileStream outFileStream = new FileStream(gvFile, FileMode.Create);
            StreamReader sr = new StreamReader(inFileStream);
            StreamWriter sw = new StreamWriter(outFileStream);

            sw.WriteLine("digraph G {");               // 有向图
            sw.WriteLine("\tgraph[rankdir=BT];");      // 指定绘制方向：从下->上

            String line;
            while ((line = sr.ReadLine()) != null)
            {
                string[] sArray = line.Split(new char[2] { ' ', '\t' });
                                                       // 提取前3个：分支号, 始点编号，末点编号
                if(hasTag)                             // 是否添加节点标签'v'和分支标签'e'
                {
                    sw.WriteLine(String.Format("\tv{1} -> v{2} [label=\"e{0}\"]", 
                        sArray[0], sArray[1], sArray[2]));
                }
                else
                {
                    sw.WriteLine(String.Format("\t{1} -> {2} [label=\"{0}\"]", 
                        sArray[0], sArray[1], sArray[2]));
                }
            }
            sw.WriteLine("}");
            sw.Flush();
            sw.Close();
            outFileStream.Close();

            sr.Close();
            inFileStream.Close();
        }

        private void delDotFile(string gvFile, bool isDel)
        {
            if (isDel)
            {
                File.Delete(gvFile);
            }
        }

        private void chageRunButtonState(bool isChange)
        {
            if(isChange)
            {
                this.runBtn.Text = "请等待...";
                this.runBtn.Enabled = false;
            }
            else
            {
                this.runBtn.Text = "运行";
                this.runBtn.Enabled = true;
            }
        }      

        private System.Diagnostics.ProcessStartInfo buildProcessInfo(string gvDir, string gFormat, string gvFile, string saveDir)
        {
            System.Diagnostics.ProcessStartInfo Info = new System.Diagnostics.ProcessStartInfo();
            //设置外部程序名
            Info.FileName = String.Format("{0}bin\\dot.exe", gvDir);
            //设置外部程序的启动参数（命令行参数）
            Info.Arguments = String.Format("-T {0} {1} -o {2}", gFormat, gvFile, saveDir);
            Info.WorkingDirectory = Application.StartupPath;
            Info.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden; // 隐藏窗口
            return Info;
        }

        private void runBtn_Click(object sender, EventArgs e)
        {
            if (!checkData()) return;
            string gvFile = "gd.gv";
            genDotFile(gpDataTextBox.Text, gvFile, hasTagCheckBox.Checked); // 生成dot文件
            
            //声明一个程序类
            System.Diagnostics.Process   Proc;
            try
            {
                
                //启动外部程序
                Proc = System.Diagnostics.Process.Start(buildProcessInfo(
                                            gvDirTextBox.Text, 
                                            gFormatCombox.SelectedItem.ToString(), 
                                            gvFile, 
                                            saveTextBox.Text));
                if (Proc != null)
                {
                    // 修改运行按钮的状态（文字、禁用）
                    chageRunButtonState(true);
                    
                    Proc.WaitForExit();

                    chageRunButtonState(false);
                
                    MessageBox.Show("图形生成完毕!");
                    
                    // 删除gd.gv文件
                    delDotFile(gvFile, delDotFileCheckBox.Checked);
                  
                    // 打开图形所在目录
                    System.Diagnostics.Process.Start("explorer.exe", Path.GetDirectoryName(saveTextBox.Text));
                }
            }
            catch (ArgumentException ex)
            {
                MessageBox.Show(ex.Message, this.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }        
    }
}
