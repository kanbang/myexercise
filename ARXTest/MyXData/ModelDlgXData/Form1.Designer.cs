namespace MyXData.ModalDlg
{
    partial class xdataForm
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(xdataForm));
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.appNameLabel = new System.Windows.Forms.Label();
            this.selectButton = new System.Windows.Forms.Button();
            this.okButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.newAppButton = new System.Windows.Forms.Button();
            this.appNamesComboBox = new System.Windows.Forms.ComboBox();
            this.addButton = new System.Windows.Forms.Button();
            this.eraseButton = new System.Windows.Forms.Button();
            this.xdataListView = new CustomListView.ListViewEx();
            this.paramColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.valueColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.delappNameButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "ICON_ADD_FAVORITE1.ico");
            // 
            // appNameLabel
            // 
            this.appNameLabel.AutoSize = true;
            this.appNameLabel.Location = new System.Drawing.Point(4, 20);
            this.appNameLabel.Name = "appNameLabel";
            this.appNameLabel.Size = new System.Drawing.Size(83, 12);
            this.appNameLabel.TabIndex = 1;
            this.appNameLabel.Text = "已注册AppName";
            // 
            // selectButton
            // 
            this.selectButton.Location = new System.Drawing.Point(238, 13);
            this.selectButton.Name = "selectButton";
            this.selectButton.Size = new System.Drawing.Size(77, 27);
            this.selectButton.TabIndex = 3;
            this.selectButton.Text = "选择实体";
            this.selectButton.UseVisualStyleBackColor = true;
            this.selectButton.Click += new System.EventHandler(this.selectButton_Click);
            // 
            // okButton
            // 
            this.okButton.Location = new System.Drawing.Point(129, 327);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(73, 29);
            this.okButton.TabIndex = 8;
            this.okButton.Text = "确定";
            this.okButton.UseVisualStyleBackColor = true;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // cancelButton
            // 
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(237, 327);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(78, 29);
            this.cancelButton.TabIndex = 9;
            this.cancelButton.Text = "取消";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // newAppButton
            // 
            this.newAppButton.Location = new System.Drawing.Point(12, 283);
            this.newAppButton.Name = "newAppButton";
            this.newAppButton.Size = new System.Drawing.Size(80, 29);
            this.newAppButton.TabIndex = 5;
            this.newAppButton.Text = "注册AppName";
            this.newAppButton.UseVisualStyleBackColor = true;
            this.newAppButton.Click += new System.EventHandler(this.newAppButton_Click);
            // 
            // appNamesComboBox
            // 
            this.appNamesComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.appNamesComboBox.FormattingEnabled = true;
            this.appNamesComboBox.Location = new System.Drawing.Point(93, 17);
            this.appNamesComboBox.Name = "appNamesComboBox";
            this.appNamesComboBox.Size = new System.Drawing.Size(126, 20);
            this.appNamesComboBox.TabIndex = 2;
            this.appNamesComboBox.SelectedIndexChanged += new System.EventHandler(this.appNamesComboBox_SelectedIndexChanged);
            // 
            // addButton
            // 
            this.addButton.Location = new System.Drawing.Point(127, 283);
            this.addButton.Name = "addButton";
            this.addButton.Size = new System.Drawing.Size(75, 29);
            this.addButton.TabIndex = 6;
            this.addButton.Text = "增加数据";
            this.addButton.UseVisualStyleBackColor = true;
            this.addButton.Click += new System.EventHandler(this.addButton_Click);
            // 
            // eraseButton
            // 
            this.eraseButton.Location = new System.Drawing.Point(237, 283);
            this.eraseButton.Name = "eraseButton";
            this.eraseButton.Size = new System.Drawing.Size(78, 27);
            this.eraseButton.TabIndex = 7;
            this.eraseButton.Text = "删除数据";
            this.eraseButton.UseVisualStyleBackColor = true;
            this.eraseButton.Click += new System.EventHandler(this.eraseButton_Click);
            // 
            // xdataListView
            // 
            this.xdataListView.Alignment = System.Windows.Forms.ListViewAlignment.Default;
            this.xdataListView.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.xdataListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.paramColumnHeader,
            this.valueColumnHeader});
            this.xdataListView.FullRowSelect = true;
            this.xdataListView.GridLines = true;
            this.xdataListView.Location = new System.Drawing.Point(6, 46);
            this.xdataListView.Margin = new System.Windows.Forms.Padding(5);
            this.xdataListView.MultiSelect = false;
            this.xdataListView.Name = "xdataListView";
            this.xdataListView.Size = new System.Drawing.Size(309, 232);
            this.xdataListView.SmallImageList = this.imageList1;
            this.xdataListView.TabIndex = 4;
            this.xdataListView.UseCompatibleStateImageBehavior = false;
            this.xdataListView.View = System.Windows.Forms.View.Details;
            this.xdataListView.SelectedIndexChanged += new System.EventHandler(this.xdataListView_SelectedIndexChanged);
            // 
            // paramColumnHeader
            // 
            this.paramColumnHeader.Text = "参数";
            this.paramColumnHeader.Width = 156;
            // 
            // valueColumnHeader
            // 
            this.valueColumnHeader.Text = "值";
            this.valueColumnHeader.Width = 152;
            // 
            // delappNameButton
            // 
            this.delappNameButton.Location = new System.Drawing.Point(12, 327);
            this.delappNameButton.Name = "delappNameButton";
            this.delappNameButton.Size = new System.Drawing.Size(80, 29);
            this.delappNameButton.TabIndex = 10;
            this.delappNameButton.Text = "删除AppName";
            this.delappNameButton.UseVisualStyleBackColor = true;
            this.delappNameButton.Click += new System.EventHandler(this.button1_Click);
            // 
            // xdataForm
            // 
            this.AcceptButton = this.okButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(320, 362);
            this.Controls.Add(this.delappNameButton);
            this.Controls.Add(this.eraseButton);
            this.Controls.Add(this.addButton);
            this.Controls.Add(this.appNamesComboBox);
            this.Controls.Add(this.newAppButton);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.okButton);
            this.Controls.Add(this.selectButton);
            this.Controls.Add(this.appNameLabel);
            this.Controls.Add(this.xdataListView);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "xdataForm";
            this.Text = "扩展数据";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private CustomListView.ListViewEx xdataListView;
        private System.Windows.Forms.Label appNameLabel;
        private System.Windows.Forms.Button selectButton;
        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.ColumnHeader paramColumnHeader;
        private System.Windows.Forms.ColumnHeader valueColumnHeader;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Button newAppButton;
        private System.Windows.Forms.ImageList imageList1;
        private System.Windows.Forms.ComboBox appNamesComboBox;
        private System.Windows.Forms.Button addButton;
        private System.Windows.Forms.Button eraseButton;
        private System.Windows.Forms.Button delappNameButton;
    }
}