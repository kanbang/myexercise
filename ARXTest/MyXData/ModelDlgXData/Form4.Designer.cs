namespace MyXData.ModalDlg
{
    partial class Form4
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.appNamesComboBox = new System.Windows.Forms.ComboBox();
            this.delAllXDatCheckBox = new System.Windows.Forms.CheckBox();
            this.cancelButton = new System.Windows.Forms.Button();
            this.obButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(5, 17);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(83, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "已注册AppName";
            // 
            // appNamesComboBox
            // 
            this.appNamesComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.appNamesComboBox.FormattingEnabled = true;
            this.appNamesComboBox.Location = new System.Drawing.Point(94, 14);
            this.appNamesComboBox.Name = "appNamesComboBox";
            this.appNamesComboBox.Size = new System.Drawing.Size(121, 20);
            this.appNamesComboBox.TabIndex = 1;
            // 
            // delAllXDatCheckBox
            // 
            this.delAllXDatCheckBox.AutoSize = true;
            this.delAllXDatCheckBox.Location = new System.Drawing.Point(53, 40);
            this.delAllXDatCheckBox.Name = "delAllXDatCheckBox";
            this.delAllXDatCheckBox.Size = new System.Drawing.Size(96, 16);
            this.delAllXDatCheckBox.TabIndex = 2;
            this.delAllXDatCheckBox.Text = "删除所有数据";
            this.delAllXDatCheckBox.UseVisualStyleBackColor = true;
            this.delAllXDatCheckBox.Click += new System.EventHandler(this.delAllXDatCheckBox_Click);
            // 
            // cancelButton
            // 
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(125, 73);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 4;
            this.cancelButton.Text = "取消";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // obButton
            // 
            this.obButton.Location = new System.Drawing.Point(6, 73);
            this.obButton.Name = "obButton";
            this.obButton.Size = new System.Drawing.Size(75, 23);
            this.obButton.TabIndex = 3;
            this.obButton.Text = "确定";
            this.obButton.UseVisualStyleBackColor = true;
            this.obButton.Click += new System.EventHandler(this.obButton_Click);
            // 
            // Form4
            // 
            this.AcceptButton = this.obButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(224, 108);
            this.ControlBox = false;
            this.Controls.Add(this.obButton);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.delAllXDatCheckBox);
            this.Controls.Add(this.appNamesComboBox);
            this.Controls.Add(this.label1);
            this.Name = "Form4";
            this.Text = "选择删除AppName";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox appNamesComboBox;
        private System.Windows.Forms.CheckBox delAllXDatCheckBox;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Button obButton;
    }
}