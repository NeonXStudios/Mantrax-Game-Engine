namespace garin_hub
{
    partial class MantraxHub
    {
        /// <summary>
        /// Variable del diseñador necesaria.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén usando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben desechar; false en caso contrario.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        /// <summary>
        /// Método necesario para admitir el Diseñador. No se puede modificar
        /// el contenido de este método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MantraxHub));
            this.Title = new System.Windows.Forms.Label();
            this.CloseButton = new System.Windows.Forms.PictureBox();
            this.DownloadProgress = new System.Windows.Forms.ProgressBar();
            this.ProyectsInfo = new System.Windows.Forms.Label();
            this.EngineOptions = new System.Windows.Forms.FlowLayoutPanel();
            this.DownloadEngine = new System.Windows.Forms.Label();
            this.SelectProyectsPath = new System.Windows.Forms.Label();
            this.NewProyect = new System.Windows.Forms.Label();
            this.Options = new System.Windows.Forms.Label();
            this.DownloadingInfo = new System.Windows.Forms.Label();
            this.ProyectName = new System.Windows.Forms.TextBox();
            this.TitleProyect = new System.Windows.Forms.Label();
            this.MakeProyect = new System.Windows.Forms.Button();
            this.ProyectsData = new System.Windows.Forms.TableLayoutPanel();
            ((System.ComponentModel.ISupportInitialize)(this.CloseButton)).BeginInit();
            this.EngineOptions.SuspendLayout();
            this.SuspendLayout();
            // 
            // Title
            // 
            this.Title.AutoSize = true;
            this.Title.Font = new System.Drawing.Font("Ubuntu", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Title.ForeColor = System.Drawing.Color.White;
            this.Title.Location = new System.Drawing.Point(12, 9);
            this.Title.Name = "Title";
            this.Title.Size = new System.Drawing.Size(139, 26);
            this.Title.TabIndex = 0;
            this.Title.Text = "Mantrax HUB";
            // 
            // CloseButton
            // 
            this.CloseButton.Image = ((System.Drawing.Image)(resources.GetObject("CloseButton.Image")));
            this.CloseButton.Location = new System.Drawing.Point(841, 12);
            this.CloseButton.Name = "CloseButton";
            this.CloseButton.Size = new System.Drawing.Size(24, 24);
            this.CloseButton.TabIndex = 1;
            this.CloseButton.TabStop = false;
            this.CloseButton.Click += new System.EventHandler(this.pictureBox1_Click);
            // 
            // DownloadProgress
            // 
            this.DownloadProgress.Location = new System.Drawing.Point(12, 437);
            this.DownloadProgress.Name = "DownloadProgress";
            this.DownloadProgress.Size = new System.Drawing.Size(853, 23);
            this.DownloadProgress.TabIndex = 2;
            // 
            // ProyectsInfo
            // 
            this.ProyectsInfo.AutoSize = true;
            this.ProyectsInfo.Font = new System.Drawing.Font("Ubuntu", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ProyectsInfo.ForeColor = System.Drawing.Color.White;
            this.ProyectsInfo.Location = new System.Drawing.Point(395, 75);
            this.ProyectsInfo.Name = "ProyectsInfo";
            this.ProyectsInfo.Size = new System.Drawing.Size(97, 26);
            this.ProyectsInfo.TabIndex = 4;
            this.ProyectsInfo.Text = "Proyects";
            // 
            // EngineOptions
            // 
            this.EngineOptions.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(35)))), ((int)(((byte)(35)))));
            this.EngineOptions.Controls.Add(this.DownloadEngine);
            this.EngineOptions.Controls.Add(this.SelectProyectsPath);
            this.EngineOptions.Controls.Add(this.NewProyect);
            this.EngineOptions.Location = new System.Drawing.Point(12, 104);
            this.EngineOptions.Name = "EngineOptions";
            this.EngineOptions.Size = new System.Drawing.Size(180, 297);
            this.EngineOptions.TabIndex = 5;
            // 
            // DownloadEngine
            // 
            this.DownloadEngine.AutoSize = true;
            this.DownloadEngine.Font = new System.Drawing.Font("Ubuntu", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.DownloadEngine.ForeColor = System.Drawing.Color.White;
            this.DownloadEngine.Location = new System.Drawing.Point(3, 10);
            this.DownloadEngine.Margin = new System.Windows.Forms.Padding(3, 10, 3, 0);
            this.DownloadEngine.Name = "DownloadEngine";
            this.DownloadEngine.Size = new System.Drawing.Size(122, 18);
            this.DownloadEngine.TabIndex = 0;
            this.DownloadEngine.Text = "Download Engine";
            this.DownloadEngine.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.DownloadEngine.Click += new System.EventHandler(this.DownloadEngine_Click);
            // 
            // SelectProyectsPath
            // 
            this.SelectProyectsPath.AutoSize = true;
            this.SelectProyectsPath.Font = new System.Drawing.Font("Ubuntu", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SelectProyectsPath.ForeColor = System.Drawing.Color.White;
            this.SelectProyectsPath.Location = new System.Drawing.Point(3, 38);
            this.SelectProyectsPath.Margin = new System.Windows.Forms.Padding(3, 10, 3, 0);
            this.SelectProyectsPath.Name = "SelectProyectsPath";
            this.SelectProyectsPath.Size = new System.Drawing.Size(104, 18);
            this.SelectProyectsPath.TabIndex = 1;
            this.SelectProyectsPath.Text = "Project Folder";
            this.SelectProyectsPath.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.SelectProyectsPath.Click += new System.EventHandler(this.label1_Click_2);
            // 
            // NewProyect
            // 
            this.NewProyect.AutoSize = true;
            this.NewProyect.Font = new System.Drawing.Font("Ubuntu", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.NewProyect.ForeColor = System.Drawing.Color.White;
            this.NewProyect.Location = new System.Drawing.Point(3, 66);
            this.NewProyect.Margin = new System.Windows.Forms.Padding(3, 10, 3, 0);
            this.NewProyect.Name = "NewProyect";
            this.NewProyect.Size = new System.Drawing.Size(93, 18);
            this.NewProyect.TabIndex = 2;
            this.NewProyect.Text = "New Proyect";
            this.NewProyect.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.NewProyect.Click += new System.EventHandler(this.NewProyect_Click);
            // 
            // Options
            // 
            this.Options.AutoSize = true;
            this.Options.Font = new System.Drawing.Font("Ubuntu", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Options.ForeColor = System.Drawing.Color.White;
            this.Options.Location = new System.Drawing.Point(55, 66);
            this.Options.Name = "Options";
            this.Options.Size = new System.Drawing.Size(88, 26);
            this.Options.TabIndex = 6;
            this.Options.Text = "Options";
            // 
            // DownloadingInfo
            // 
            this.DownloadingInfo.AutoSize = true;
            this.DownloadingInfo.Font = new System.Drawing.Font("Ubuntu", 7F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.DownloadingInfo.ForeColor = System.Drawing.Color.White;
            this.DownloadingInfo.Location = new System.Drawing.Point(12, 419);
            this.DownloadingInfo.Name = "DownloadingInfo";
            this.DownloadingInfo.Size = new System.Drawing.Size(10, 15);
            this.DownloadingInfo.TabIndex = 7;
            this.DownloadingInfo.Text = ".";
            // 
            // ProyectName
            // 
            this.ProyectName.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(35)))), ((int)(((byte)(35)))));
            this.ProyectName.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.ProyectName.Font = new System.Drawing.Font("Ubuntu", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ProyectName.ForeColor = System.Drawing.Color.White;
            this.ProyectName.Location = new System.Drawing.Point(695, 142);
            this.ProyectName.Name = "ProyectName";
            this.ProyectName.Size = new System.Drawing.Size(170, 13);
            this.ProyectName.TabIndex = 9;
            this.ProyectName.Text = "New Proyect";
            this.ProyectName.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // TitleProyect
            // 
            this.TitleProyect.AutoSize = true;
            this.TitleProyect.Font = new System.Drawing.Font("Ubuntu", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TitleProyect.ForeColor = System.Drawing.Color.White;
            this.TitleProyect.Location = new System.Drawing.Point(708, 102);
            this.TitleProyect.Name = "TitleProyect";
            this.TitleProyect.Size = new System.Drawing.Size(147, 26);
            this.TitleProyect.TabIndex = 10;
            this.TitleProyect.Text = "Proyect Name";
            // 
            // MakeProyect
            // 
            this.MakeProyect.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
            this.MakeProyect.FlatAppearance.BorderSize = 0;
            this.MakeProyect.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.MakeProyect.Font = new System.Drawing.Font("Ubuntu", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.MakeProyect.ForeColor = System.Drawing.Color.White;
            this.MakeProyect.Location = new System.Drawing.Point(713, 170);
            this.MakeProyect.Name = "MakeProyect";
            this.MakeProyect.Size = new System.Drawing.Size(142, 23);
            this.MakeProyect.TabIndex = 11;
            this.MakeProyect.Text = "Create Proyect";
            this.MakeProyect.UseVisualStyleBackColor = false;
            this.MakeProyect.Click += new System.EventHandler(this.MakeProyect_Click);
            // 
            // ProyectsData
            // 
            this.ProyectsData.AccessibleName = "ProyectsData";
            this.ProyectsData.AccessibleRole = System.Windows.Forms.AccessibleRole.PageTabList;
            this.ProyectsData.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(32)))), ((int)(((byte)(32)))), ((int)(((byte)(32)))));
            this.ProyectsData.ColumnCount = 2;
            this.ProyectsData.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.ProyectsData.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.ProyectsData.Location = new System.Drawing.Point(198, 104);
            this.ProyectsData.Name = "ProyectsData";
            this.ProyectsData.RowCount = 2;
            this.ProyectsData.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.ProyectsData.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.ProyectsData.Size = new System.Drawing.Size(491, 297);
            this.ProyectsData.TabIndex = 12;
            // 
            // MantraxHub
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
            this.ClientSize = new System.Drawing.Size(877, 472);
            this.Controls.Add(this.ProyectsData);
            this.Controls.Add(this.MakeProyect);
            this.Controls.Add(this.TitleProyect);
            this.Controls.Add(this.DownloadingInfo);
            this.Controls.Add(this.Options);
            this.Controls.Add(this.ProyectName);
            this.Controls.Add(this.EngineOptions);
            this.Controls.Add(this.ProyectsInfo);
            this.Controls.Add(this.DownloadProgress);
            this.Controls.Add(this.CloseButton);
            this.Controls.Add(this.Title);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MantraxHub";
            this.Text = "Mantrax Hub";
            ((System.ComponentModel.ISupportInitialize)(this.CloseButton)).EndInit();
            this.EngineOptions.ResumeLayout(false);
            this.EngineOptions.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label Title;
        private System.Windows.Forms.PictureBox CloseButton;
        private System.Windows.Forms.ProgressBar DownloadProgress;
        private System.Windows.Forms.Label ProyectsInfo;
        private System.Windows.Forms.FlowLayoutPanel EngineOptions;
        private System.Windows.Forms.Label Options;
        private System.Windows.Forms.Label DownloadingInfo;
        private System.Windows.Forms.Label DownloadEngine;
        private System.Windows.Forms.Label SelectProyectsPath;
        private System.Windows.Forms.Label NewProyect;
        private System.Windows.Forms.TextBox ProyectName;
        private System.Windows.Forms.Label TitleProyect;
        private System.Windows.Forms.Button MakeProyect;
        private System.Windows.Forms.TableLayoutPanel ProyectsData;
    }
}

