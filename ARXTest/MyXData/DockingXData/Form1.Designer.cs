namespace MyXData.DockingPalette
{
    partial class xdataForm
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

        #region 组件设计器生成的代码

        /// <summary> 
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(xdataForm));
            this.appNameLabel = new System.Windows.Forms.Label();
            this.appNamesComboBox = new System.Windows.Forms.ComboBox();
            this.selectButton = new System.Windows.Forms.Button();
            this.xdataListView = new CustomListView.ListViewEx();
            this.paramColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.valueColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.newAppButton = new System.Windows.Forms.Button();
            this.addButton = new System.Windows.Forms.Button();
            this.okButton = new System.Windows.Forms.Button();
            this.delappNameButton = new System.Windows.Forms.Button();
            this.eraseButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // appNameLabel
            // 
            this.appNameLabel.AutoSize = true;
            this.appNameLabel.Location = new System.Drawing.Point(11, 15);
            this.appNameLabel.Name = "appNameLabel";
            this.appNameLabel.Size = new System.Drawing.Size(83, 12);
            this.appNameLabel.TabIndex = 0;
            this.appNameLabel.Text = "已注册AppName";
            // 
            // appNamesComboBox
            // 
            this.appNamesComboBox.FormattingEnabled = true;
            this.appNamesComboBox.Location = new System.Drawing.Point(94, 12);
            this.appNamesComboBox.Name = "appNamesComboBox";
            this.appNamesComboBox.Size = new System.Drawing.Size(128, 20);
            this.appNamesComboBox.TabIndex = 1;
            this.appNamesComboBox.SelectedIndexChanged += new System.EventHandler(this.appNamesComboBox_SelectedIndexChanged);
            this.appNamesComboBox.DropDownClosed += new System.EventHandler(this.appNamesComboBox_DropDownClosed);
            this.appNamesComboBox.DropDown += new System.EventHandler(this.appNamesComboBox_DropDown);
            // 
            // selectButton
            // 
            this.selectButton.Location = new System.Drawing.Point(231, 10);
            this.selectButton.Name = "selectButton";
            this.selectButton.Size = new System.Drawing.Size(82, 23);
            this.selectButton.TabIndex = 2;
            this.selectButton.Text = "选择实体";
            this.selectButton.UseVisualStyleBackColor = true;
            this.selectButton.Click += new System.EventHandler(this.selectButton_Click);
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
            this.xdataListView.Location = new System.Drawing.Point(13, 39);
            this.xdataListView.MultiSelect = false;
            this.xdataListView.Name = "xdataListView";
            this.xdataListView.Size = new System.Drawing.Size(300, 367);
            this.xdataListView.SmallImageList = this.imageList1;
            this.xdataListView.TabIndex = 3;
            this.xdataListView.UseCompatibleStateImageBehavior = false;
            this.xdataListView.View = System.Windows.Forms.View.Details;
            this.xdataListView.SelectedIndexChanged += new System.EventHandler(this.xdataListView_SelectedIndexChanged);
            // 
            // paramColumnHeader
            // 
            this.paramColumnHeader.Text = "参数";
            this.paramColumnHeader.Width = 140;
            // 
            // valueColumnHeader
            // 
            this.valueColumnHeader.Text = "值";
            this.valueColumnHeader.Width = 155;
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "ICON_ADD_FAVORITE1.ico");
            // 
            // newAppButton
            // 
            this.newAppButton.Location = new System.Drawing.Point(13, 430);
            this.newAppButton.Name = "newAppButton";
            this.newAppButton.Size = new System.Drawing.Size(88, 23);
            this.newAppButton.TabIndex = 4;
            this.newAppButton.Text = "注册AppName";
            this.newAppButton.UseVisualStyleBackColor = true;
            this.newAppButton.Click += new System.EventHandler(this.newAppButton_Click);
            // 
            // addButton
            // 
            this.addButton.Location = new System.Drawing.Point(124, 430);
            this.addButton.Name = "addButton";
            this.addButton.Size = new System.Drawing.Size(82, 23);
            this.addButton.TabIndex = 5;
            this.addButton.Text = "增加数据";
            this.addButton.UseVisualStyleBackColor = true;
            this.addButton.Click += new System.EventHandler(this.addButton_Click);
            // 
            // okButton
            // 
            this.okButton.Location = new System.Drawing.Point(227, 430);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(86, 58);
            this.okButton.TabIndex = 6;
            this.okButton.Text = "更新数据";
            this.okButton.UseVisualStyleBackColor = true;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // delappNameButton
            // 
            this.delappNameButton.Location = new System.Drawing.Point(13, 465);
            this.delappNameButton.Name = "delappNameButton";
            this.delappNameButton.Size = new System.Drawing.Size(88, 26);
            this.delappNameButton.TabIndex = 7;
            this.delappNameButton.Text = "删除AppName";
            this.delappNameButton.UseVisualStyleBackColor = true;
            this.delappNameButton.Click += new System.EventHandler(this.button1_Click);
            // 
            // eraseButton
            // 
            this.eraseButton.Location = new System.Drawing.Point(124, 465);
            this.eraseButton.Name = "eraseButton";
            this.eraseButton.Size = new System.Drawing.Size(82, 23);
            this.eraseButton.TabIndex = 8;
            this.eraseButton.Text = "删除数据";
            this.eraseButton.UseVisualStyleBackColor = true;
            this.eraseButton.Click += new System.EventHandler(this.eraseButton_Click);
            // 
            // xdataForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.Controls.Add(this.eraseButton);
            this.Controls.Add(this.delappNameButton);
            this.Controls.Add(this.okButton);
            this.Controls.Add(this.addButton);
            this.Controls.Add(this.newAppButton);
            this.Controls.Add(this.xdataListView);
            this.Controls.Add(this.selectButton);
            this.Controls.Add(this.appNamesComboBox);
            this.Controls.Add(this.appNameLabel);
            this.Name = "xdataForm";
            this.Size = new System.Drawing.Size(316, 494);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label appNameLabel;
        private System.Windows.Forms.ComboBox appNamesComboBox;
        private System.Windows.Forms.Button selectButton;
        private CustomListView.ListViewEx xdataListView;
        private System.Windows.Forms.Button newAppButton;
        private System.Windows.Forms.Button addButton;
        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.ColumnHeader paramColumnHeader;
        private System.Windows.Forms.ColumnHeader valueColumnHeader;
        private System.Windows.Forms.Button delappNameButton;
        private System.Windows.Forms.Button eraseButton;
        private System.Windows.Forms.ImageList imageList1;
    }
}
