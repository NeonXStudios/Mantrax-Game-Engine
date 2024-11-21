using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Security.Cryptography;
using System.Threading.Tasks;
using System.Windows.Forms;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using SharpCompress.Archives;


namespace garin_hub
{
    public partial class MantraxHub : Form
    {
        private bool isDragging = false;
        private Point startPoint = new Point(0, 0);

        public string _ProyectsPath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
        public string _ProgramPath = "NULL";
        public string _ProgramPathNonExe = "NULL";

        public string DownloadUrl = "https://firebasestorage.googleapis.com/v0/b/neonxstudios-tie-1716852545375" +
                                                                ".appspot.com/o/GarinEngine%2FMantraxEngine.zip?alt=media&token=34be967" +
                                                                 "5-c112-47fc-b1c4-58881ef62d87";

        public MantraxHub()
        {
            InitializeComponent();

            this.MinimizeBox = false;
            this.MaximizeBox = false;
            this.ControlBox = false;

            this.FormBorderStyle = FormBorderStyle.None;

            this.MouseDown += new MouseEventHandler(Form1_MouseDown);
            this.MouseMove += new MouseEventHandler(Form1_MouseMove);
            this.MouseUp += new MouseEventHandler(Form1_MouseUp);

            CloseButton.SizeMode = PictureBoxSizeMode.Zoom;

            EngineOptions.FlowDirection = FlowDirection.LeftToRight;
            EngineOptions.WrapContents = true;
            EngineOptions.Padding = new Padding(25, 10, 0, 0);

            DownloadProgress.Style = ProgressBarStyle.Continuous;

            DownloadProgress.Visible = false;
            DownloadingInfo.Visible = false;

            DownloadingInfo.Text = "";

            if (!File.Exists("Data.json"))
            {
                SaveData();
            }
            else
            {
                LoadData();
            }

            SetupProyects();
            DisableNewProyect();

            ProyectName.Multiline = true;
            ProyectName.Height = 20;

            ProyectsData.RowCount = 5;  
            ProyectsData.ColumnCount = 1; 
        }

        public void SetupProyects()
        {
            ProyectsData.Controls.Clear();

            if (Directory.Exists(_ProyectsPath))
            {
                string[] directories = Directory.GetDirectories(_ProyectsPath);

                foreach (var directory in directories)
                {
                    string gameSettingsPath = Path.Combine(directory, "GameSettings.data");
                    if (File.Exists(gameSettingsPath))
                    {
                        Label projectLabel = new Label
                        {
                            Text = Path.GetFileName(directory),
                            Margin = new Padding(0, 10, 0, 0),
                            Font = new Font("Ubuntu", 15, FontStyle.Bold),
                            ForeColor = Color.White,
                            BackColor = Color.FromArgb(255, 42, 42, 42),
                            Width = 300,
                            Height = 30
                        };

                        projectLabel.Click += (sender, e) => OpenProgramWithProject(directory);

                        ProyectsData.Controls.Add(projectLabel);
                    }
                }
            }
            else
            {
                MessageBox.Show("Please install Mantrax Engine before starting a project.");
            }
        }



        private void OpenProgramWithProject(string projectPath)
        {
            if (File.Exists(_ProgramPath))
            {
                try
                {
                    string formattedProjectPath = Path.Combine(projectPath, "");

                    ProcessStartInfo startInfo = new ProcessStartInfo();
                    startInfo.FileName = _ProgramPath;  
                    startInfo.Arguments = formattedProjectPath;  

                    string programDirectory = Path.GetDirectoryName(_ProgramPath);  
                    startInfo.WorkingDirectory = programDirectory;  

                    Process.Start(startInfo);
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Error in execution: {ex.Message}");
                }
            }
            else
            {
                MessageBox.Show("Please install Mantrax Engine before starting a project.");
            }
        }



        private void Form1_MouseDown(object sender, MouseEventArgs e)
        {
            isDragging = true;
            startPoint = new Point(e.X, e.Y);
        }

        private void Form1_MouseMove(object sender, MouseEventArgs e)
        {
            if (isDragging)
            {
                Point p = PointToScreen(e.Location);
                this.Location = new Point(p.X - startPoint.X, p.Y - startPoint.Y);
            }
        }

        private void Form1_MouseUp(object sender, MouseEventArgs e)
        {
            isDragging = false;
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            this.Close();
        }


        private async void DownloadEngine_Click(object sender, EventArgs e)
        {
            DownloadProgress.Visible = true;
            DownloadingInfo.Visible = true;

            DownloadingInfo.Text = "Getting data from the server...";

            await Task.Delay(3000);

            string save_to = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), "Engine.rar");
            string save_to_extract = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments));

            DownloadProgress.Value = 0;
            DownloadProgress.Minimum = 0;
            DownloadProgress.Maximum = 100;

            using (var httpClient = new HttpClient())
            {
                httpClient.Timeout = TimeSpan.FromMinutes(10);

                using (var response = await httpClient.GetAsync(DownloadUrl, HttpCompletionOption.ResponseHeadersRead))
                {
                    response.EnsureSuccessStatusCode();

                    var contentLength = response.Content.Headers.ContentLength;

                    using (var contentStream = await response.Content.ReadAsStreamAsync())
                    using (var fileStream = new FileStream(save_to, FileMode.Create, FileAccess.Write, FileShare.None, 8191, true))
                    {
                        var totalRead = 0L;
                        var buffer = new byte[8192];
                        int bytesRead;

                        while ((bytesRead = await contentStream.ReadAsync(buffer, 0, buffer.Length)) > 0)
                        {
                            await fileStream.WriteAsync(buffer, 0, bytesRead);
                            totalRead += bytesRead;

                            if (contentLength.HasValue)
                            {
                                var progress = (int)((totalRead * 100) / contentLength.Value);
                                DownloadProgress.Value = progress;
                                DownloadingInfo.Text = $"Downloading Engine ({DownloadProgress.Value}%)...";
                                DownloadProgress.Update();
                            }
                        }
                    }
                }
            }

            DownloadingInfo.Text = "Download completed (WAIT)...";

            await Task.Delay(3000);

            DownloadingInfo.Text = "Unpacking...";

            await Task.Delay(3000);

            string archivoRar = save_to;
            string destino = save_to_extract;

            Directory.CreateDirectory(destino);

            try
            {
                using (var archivo = ArchiveFactory.Open(archivoRar))
                {
                    string destinoFinal = Path.Combine(destino, "MantraxEngine");

                    if (Directory.Exists(destinoFinal))
                    {
                        Directory.Delete(destinoFinal, true);
                    }

                    Directory.CreateDirectory(destinoFinal);

                    foreach (var entry in archivo.Entries)
                    {
                        if (!entry.IsDirectory)
                        {
                            string archivoDestino = Path.Combine(destinoFinal, entry.Key);

                            string directorioDestino = Path.GetDirectoryName(archivoDestino);
                            if (!Directory.Exists(directorioDestino))
                            {
                                Directory.CreateDirectory(directorioDestino);
                            }

                            using (var stream = File.Create(archivoDestino))
                            {
                                entry.WriteTo(stream);
                            }

                            _ProgramPath = $"{destinoFinal}/Mantrax_Engine.exe";
                            DownloadingInfo.Text = "Mantrax Unpacked...";
                        }
                    }
                }


                await Task.Delay(1000);

                DownloadingInfo.Text = "Starting file verification in case of error...";

                try
                {
                    using (var archivo = ArchiveFactory.Open(save_to))
                    {
                        if (archivo.IsSolid)
                        {
                            DownloadingInfo.Text = "All files are correct...";
                        }
                        else
                        {
                            DownloadingInfo.Text = "The file was found to be corrupt...";
                        }
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"El archivo .rar está corrupto o no se puede abrir. Error: {ex.Message}");
                }

                await Task.Delay(1000);

                DownloadingInfo.Text = "Clearing temporary files and finishing installation...";
                
                if (File.Exists(save_to))
                {
                    File.Delete(save_to);
                }

                DownloadingInfo.Text = "Clearing temporary files and finishing installation...";

                await Task.Delay(1000);

                if (!File.Exists(save_to))
                {
                    DownloadingInfo.Text = "Temporary files cleaned...";
                }

                await Task.Delay(3000);

                DownloadingInfo.Text = "Mantrax Engine Installed Successfully...";
                SaveData();

                DownloadProgress.Visible = false;
                DownloadingInfo.Visible = false;
            }
            catch (Exception ex)
            {  
                DownloadingInfo.Text = $"Error during unpacking {ex.Message}";
            }
        }

        static string CalcularHash(string archivo)
        {
            using (var md5 = MD5.Create())
            {
                using (var stream = File.OpenRead(archivo))
                {
                    byte[] hashBytes = md5.ComputeHash(stream);
                    return BitConverter.ToString(hashBytes).Replace("-", "").ToLower();
                }
            }
        }

        private void label1_Click_2(object sender, EventArgs e)
        {
            using (FolderBrowserDialog folderDialog = new FolderBrowserDialog())
            {
                folderDialog.Description = "Select a folder where you will save your projects";

                DialogResult result = folderDialog.ShowDialog();

                if (result == DialogResult.OK)
                {
                    string pathSelect = folderDialog.SelectedPath;

                    _ProyectsPath = pathSelect;
                    SaveData();

                    SetupProyects();
                }
                else
                {
                    Console.WriteLine($"Error On Select Path");
                }
            }
        }

        public void SaveData()
        {
            JObject data_hub = new JObject();
            data_hub["ProyectPath"] = _ProyectsPath;
            data_hub["EnginePath"] = _ProgramPath;
            
            string json_data = JsonConvert.SerializeObject(data_hub, Formatting.Indented);
            string appDirectory = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Data.json");

            File.WriteAllText(appDirectory, json_data);
        }


        public void LoadData()
        {
            string appDirectory = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Data.json");

            if (File.Exists(appDirectory))
            {
                string json_data = File.ReadAllText(appDirectory);

                JObject data_hub = JsonConvert.DeserializeObject<JObject>(json_data);

                if (data_hub["ProyectPath"] != null)
                    _ProyectsPath = data_hub["ProyectPath"].ToString();

                if (data_hub["EnginePath"] != null)
                    _ProgramPath = data_hub["EnginePath"].ToString();
            }
            else
            {
                Console.WriteLine("El archivo no existe.");
            }
        }

        private void NewProyect_Click(object sender, EventArgs e)
        {
            EnableNewProyect();
        }

        private void MakeProyect_Click(object sender, EventArgs e)
        { 
            string rar_path = Path.Combine(Path.GetDirectoryName(_ProgramPath), "data", "WindowBin.zip");
            string destinoFinal = Path.Combine(_ProyectsPath, ProyectName.Text);

            if (Directory.Exists(destinoFinal))
            {
                return;
            }

            DownloadProgress.Visible = true;

            using (var archivo = ArchiveFactory.Open(rar_path))
            {
                if (Directory.Exists(destinoFinal))
                {
                    Directory.Delete(destinoFinal, true);
                }

                Directory.CreateDirectory(destinoFinal);

                int totalEntries = archivo.Entries.Count(entry => !entry.IsDirectory);
                int processedEntries = 0;

                foreach (var entry in archivo.Entries)
                {
                    if (!entry.IsDirectory)
                    {
                        string archivoDestino = Path.Combine(destinoFinal, entry.Key);
                        string directorioDestino = Path.GetDirectoryName(archivoDestino);

                        if (!Directory.Exists(directorioDestino))
                        {
                            Directory.CreateDirectory(directorioDestino);
                        }

                        using (var stream = File.Create(archivoDestino))
                        {
                            entry.WriteTo(stream);
                        }

                        processedEntries++;
                        double progress = (double)processedEntries / totalEntries * 100;

                        DownloadProgress.Value = (int)progress;
                        DownloadProgress.Update();

                        Console.WriteLine($"Extraction Progress: {progress:F2}%");
                    }
                }
            }

            DownloadProgress.Visible = false;
            DisableNewProyect();
            SetupProyects();
        }


        public void DisableNewProyect()
        {
            TitleProyect.Visible = false;
            ProyectName.Visible = false;
            MakeProyect.Visible = false;
        }

        public void EnableNewProyect()
        {
            TitleProyect.Visible = true;
            ProyectName.Visible = true;
            MakeProyect.Visible = true;
        }
    }
}