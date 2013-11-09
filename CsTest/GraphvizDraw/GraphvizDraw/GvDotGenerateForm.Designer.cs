namespace GraphvizDraw
{
    partial class GvDotGenerateForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.gvDirTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.gFormatCombox = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.gpDataTextBox = new System.Windows.Forms.TextBox();
            this.selGPDataBtn = new System.Windows.Forms.Button();
            this.selSaveDirBtn = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.saveTextBox = new System.Windows.Forms.TextBox();
            this.runBtn = new System.Windows.Forms.Button();
            this.hasTagCheckBox = new System.Windows.Forms.CheckBox();
            this.delDotFileCheckBox = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(137, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "指定Graphviz的安装目录";
            // 
            // gvDirTextBox
            // 
            this.gvDirTextBox.Location = new System.Drawing.Point(14, 44);
            this.gvDirTextBox.Name = "gvDirTextBox";
            this.gvDirTextBox.ReadOnly = true;
            this.gvDirTextBox.Size = new System.Drawing.Size(462, 21);
            this.gvDirTextBox.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(18, 91);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(77, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "指定图形格式";
            // 
            // gFormatCombox
            // 
            this.gFormatCombox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.gFormatCombox.FormattingEnabled = true;
            this.gFormatCombox.Location = new System.Drawing.Point(101, 88);
            this.gFormatCombox.Name = "gFormatCombox";
            this.gFormatCombox.Size = new System.Drawing.Size(83, 20);
            this.gFormatCombox.TabIndex = 4;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(25, 139);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(137, 12);
            this.label3.TabIndex = 5;
            this.label3.Text = "请选择一个拓扑数据文件";
            // 
            // gpDataTextBox
            // 
            this.gpDataTextBox.Location = new System.Drawing.Point(25, 162);
            this.gpDataTextBox.Name = "gpDataTextBox";
            this.gpDataTextBox.Size = new System.Drawing.Size(381, 21);
            this.gpDataTextBox.TabIndex = 6;
            // 
            // selGPDataBtn
            // 
            this.selGPDataBtn.Location = new System.Drawing.Point(420, 154);
            this.selGPDataBtn.Name = "selGPDataBtn";
            this.selGPDataBtn.Size = new System.Drawing.Size(61, 34);
            this.selGPDataBtn.TabIndex = 2;
            this.selGPDataBtn.Text = "选择...";
            this.selGPDataBtn.UseVisualStyleBackColor = true;
            this.selGPDataBtn.Click += new System.EventHandler(this.selGPDataBtn_Click);
            // 
            // selSaveDirBtn
            // 
            this.selSaveDirBtn.Location = new System.Drawing.Point(422, 214);
            this.selSaveDirBtn.Name = "selSaveDirBtn";
            this.selSaveDirBtn.Size = new System.Drawing.Size(61, 34);
            this.selSaveDirBtn.TabIndex = 2;
            this.selSaveDirBtn.Text = "选择...";
            this.selSaveDirBtn.UseVisualStyleBackColor = true;
            this.selSaveDirBtn.Click += new System.EventHandler(this.selSaveDirBtn_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(27, 199);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(125, 12);
            this.label4.TabIndex = 5;
            this.label4.Text = "请选择保存图形的位置";
            // 
            // saveTextBox
            // 
            this.saveTextBox.Location = new System.Drawing.Point(27, 222);
            this.saveTextBox.Name = "saveTextBox";
            this.saveTextBox.Size = new System.Drawing.Size(381, 21);
            this.saveTextBox.TabIndex = 6;
            // 
            // runBtn
            // 
            this.runBtn.Location = new System.Drawing.Point(79, 267);
            this.runBtn.Name = "runBtn";
            this.runBtn.Size = new System.Drawing.Size(314, 64);
            this.runBtn.TabIndex = 7;
            this.runBtn.Text = "运行";
            this.runBtn.UseVisualStyleBackColor = true;
            this.runBtn.Click += new System.EventHandler(this.runBtn_Click);
            // 
            // hasTagCheckBox
            // 
            this.hasTagCheckBox.AutoSize = true;
            this.hasTagCheckBox.Location = new System.Drawing.Point(223, 91);
            this.hasTagCheckBox.Name = "hasTagCheckBox";
            this.hasTagCheckBox.Size = new System.Drawing.Size(132, 16);
            this.hasTagCheckBox.TabIndex = 8;
            this.hasTagCheckBox.Text = "节点和分支带有标签";
            this.hasTagCheckBox.UseVisualStyleBackColor = true;
            // 
            // delDotFileCheckBox
            // 
            this.delDotFileCheckBox.AutoSize = true;
            this.delDotFileCheckBox.Location = new System.Drawing.Point(362, 91);
            this.delDotFileCheckBox.Name = "delDotFileCheckBox";
            this.delDotFileCheckBox.Size = new System.Drawing.Size(114, 16);
            this.delDotFileCheckBox.TabIndex = 9;
            this.delDotFileCheckBox.Text = "删除dot临时文件";
            this.delDotFileCheckBox.UseVisualStyleBackColor = true;
            // 
            // GvDotGenerateForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(493, 336);
            this.Controls.Add(this.delDotFileCheckBox);
            this.Controls.Add(this.hasTagCheckBox);
            this.Controls.Add(this.runBtn);
            this.Controls.Add(this.saveTextBox);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.gpDataTextBox);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.gFormatCombox);
            this.Controls.Add(this.selSaveDirBtn);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.selGPDataBtn);
            this.Controls.Add(this.gvDirTextBox);
            this.Controls.Add(this.label1);
            this.Name = "GvDotGenerateForm";
            this.Text = "Graphviz网络图生成程序";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox gvDirTextBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox gFormatCombox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox gpDataTextBox;
        private System.Windows.Forms.Button selGPDataBtn;
        private System.Windows.Forms.Button selSaveDirBtn;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox saveTextBox;
        private System.Windows.Forms.Button runBtn;
        private System.Windows.Forms.CheckBox hasTagCheckBox;
        private System.Windows.Forms.CheckBox delDotFileCheckBox;
    }
}

