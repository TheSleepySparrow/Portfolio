using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Project_AP
{
    public partial class EquipmentForm : Form
    {
        RackService rackService = new("https://helow19274.ru/aip/api/rack", "5RNYBdLduTDxQCcM8YYrb5nA:H4dScAyGbS89KgLgZBs2vPsk");
        public EquipmentForm()
        {
            InitializeComponent();
            this.WindowState = FormWindowState.Maximized;
        }
        private void EquipmentForm_Paint(object sender, PaintEventArgs e)
        {
            ControlPaint.DrawBorder(e.Graphics, ClientRectangle, Color.FromArgb(143, 142, 191), ButtonBorderStyle.Solid);
        }

        private async void EquipmentForm_Load(object sender, EventArgs e)
        {
            // Получение данных из Tag и использование их
            if (Tag != null)
            {
                int hardware_id = (int)Tag;
                API api = new API();
                try
                {
                    Hardware hardware = await api.GetAllInfoHardware(hardware_id);
                    int height = (int)(flowLayoutPanel1.Height / 7);
                    for (int i = 1; i < 4; i++)
                    {
                        Panel panel = new()
                        {
                            BackColor = Color.White,
                            Margin = new Padding(10),
                            Size = new Size(flowLayoutPanel1.Width, height),
                            Padding = new Padding(10, 0, 10, 0),
                            Tag = i
                        };

                        Label label = new()
                        {
                            TextAlign = ContentAlignment.MiddleCenter,
                            Dock = DockStyle.Fill,
                        };
                        switch (panel.Tag)
                        {
                            case 1:
                                label.Text = hardware.name;
                                label.Font = new Font("Segoe UI", 32F, FontStyle.Regular, GraphicsUnit.Point);
                                break;
                            case 2:
                                label.Text = "Тип оборудования: " + hardware.type;
                                label.Font = new Font("Segoe UI", 24F, FontStyle.Regular, GraphicsUnit.Point);
                                label.ForeColor = Color.FromArgb(143, 142, 191);
                                break;
                            case 3:
                                if (string.IsNullOrEmpty(hardware.description))
                                {
                                    label.Text = "Описания нет в базе данных";
                                }
                                else
                                {
                                    label.Text = "Описание: " + hardware.description;
                                }
                                label.Font = new Font("Segoe UI", 20F, FontStyle.Regular, GraphicsUnit.Point);
                                break;
                        }
                        
                        panel.Controls.Add(label);

                        
                        flowLayoutPanel1.Controls.Add(panel);

                    }
                    Button specificationButton = new()
                    {
                            FlatStyle = FlatStyle.Flat,
                            BackColor = Color.FromArgb(143, 142, 191),
                            Text = "Спецификация",
                            Size = new Size(flowLayoutPanel1.Width, height),
                            Padding = new Padding(10, 0, 10, 0),
                    };
                    specificationButton.Click += (sender, e) =>
                    {
                        Form popupForm = new Form();
                        popupForm.Size = new System.Drawing.Size(450, 450);
                        popupForm.ShowIcon = false;
                        int i = 0;
                        TableLayoutPanel specList = new()
                        {
                            Dock = DockStyle.Fill,
                            CellBorderStyle = TableLayoutPanelCellBorderStyle.Inset,
                            ColumnCount = 2,
                            RowCount = 1,
                            Font = new Font("Segoe UI", 12F, FontStyle.Regular, GraphicsUnit.Point),
                            AutoScroll = true,
                            Margin = new Padding(10, 5, 10, 5),
                        };
                        specList.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 60F));
                        specList.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 40F));

                        if (hardware.specifications.Count != 0)
                        {
                            foreach (KeyValuePair<string, int> pair in hardware.specifications)
                            {
                                Label key = new()
                                {
                                    Text = pair.Key,
                                    TextAlign = ContentAlignment.MiddleLeft,
                                    Dock = DockStyle.Fill,
                                };
                                Label value = new()
                                {
                                    Text = pair.Value.ToString(),
                                    TextAlign = ContentAlignment.MiddleCenter,
                                    Dock = DockStyle.Fill
                                };
                                specList.Controls.Add(key, 0, i);
                                specList.Controls.Add(value, 1, i);

                                specList.RowCount++;
                                specList.RowStyles.Add(new RowStyle(SizeType.Absolute, 50F));
                                i++;
                            }
                            popupForm.Controls.Add(specList);
                        }
                        else
                        {
                            Label key = new()
                            {
                                Text = "Спецификации нет",
                                TextAlign = ContentAlignment.MiddleLeft,
                                Dock = DockStyle.Fill,
                            };
                            Panel pls = new() {
                                Dock = DockStyle.Top,
                                Margin = new Padding(10, 5, 10, 5),
                             
                            };
                            pls.Controls.Add(key);
                            popupForm.Controls.Add(pls);
                        }

                        // Отображение выплывающей формы над кнопкой
                        popupForm.StartPosition = FormStartPosition.Manual;
                        int screenWidth = Screen.PrimaryScreen.Bounds.Width;
                        int screenHeight = Screen.PrimaryScreen.Bounds.Height;
                        int popupLeft = (screenWidth - popupForm.Width) / 2;
                        int popupTop = (screenHeight - popupForm.Height) / 2;

                        // Установка координат для расположения посередине экрана
                        popupForm.Left = popupLeft;
                        popupForm.Top = popupTop;
                        popupForm.Show();
                    };
                    flowLayoutPanel1.Controls.Add(specificationButton);
                    Button locationButton = new()
                    {
                        FlatStyle = FlatStyle.Flat,
                        BackColor = Color.FromArgb(143, 142, 191),
                        Text = "Местоположение",
                        Size = new Size(flowLayoutPanel1.Width, height),
                        Padding = new Padding(10, 0, 10, 0),
                    };
                    flowLayoutPanel1.Controls.Add(locationButton);
                    locationButton.Click += async (sender, e) =>
                    {
                        EquipmentAllLocations newForm = new()
                        {
                            Size = new Size(520, 390)
                        };
                        newForm.hardware = hardware;
                        newForm.Show();
                    };

                    if (!string.IsNullOrEmpty(hardware.image_link))
                    {

                        if (Uri.TryCreate(hardware.image_link, UriKind.Absolute, result: out Uri uri) && (uri.Scheme == Uri.UriSchemeHttp || uri.Scheme == Uri.UriSchemeHttps))
                        {
                            using WebClient client = new();
                            try
                            {
                                byte[] imageData = client.DownloadData(uri);
                                using MemoryStream stream = new(imageData);
                                
                                    Image image = Image.FromStream(stream);

                                    pictureBox1.Image = image;
                                    pictureBox1.SizeMode = PictureBoxSizeMode.Zoom;
                                    pictureBox1.BackColor = Color.White;
                                    panel3.BackColor = Color.White;
                                
                            }
                            catch (Exception ex)
                            {
                                Console.WriteLine($"Ошибка загрузки изображения: {ex.Message}");
                            }
                        }
                    }

                }
                catch (Exception ex)
                {
                    MessageBox.Show($"An error occurred: {ex.Message}");
                }

            }

        }

        private void UsersLabel_MouseEnter(object sender, EventArgs e)
        {
            Label UsersLabel = (Label)sender;
            UsersLabel.BackColor = Color.FromArgb(172, 171, 221);
            UsersLabel.BorderStyle = BorderStyle.FixedSingle;
        }

        private void UsersLabel_MouseLeave(object sender, EventArgs e)
        {
            Label UsersLabel = (Label)sender;
            UsersLabel.BackColor = Color.FromArgb(143, 142, 191);
            UsersLabel.BorderStyle = BorderStyle.None;
        }

        private void StorageLabel_MouseEnter(object sender, EventArgs e)
        {
            Label StorageLabel = (Label)sender;
            StorageLabel.BackColor = Color.FromArgb(172, 171, 221);
            StorageLabel.BorderStyle = BorderStyle.FixedSingle;
        }

        private void StorageLabel_MouseLeave(object sender, EventArgs e)
        {
            Label StorageLabel = (Label)sender;
            StorageLabel.BackColor = Color.FromArgb(143, 142, 191);
            StorageLabel.BorderStyle = BorderStyle.None;
        }

        private void UsersLabel_Click(object sender, EventArgs e)
        {
            OpenNewForm();
        }

        private void StorageLabel_Click(object sender, EventArgs e)
        {
            OpenNewForm();
        }

        private static void OpenNewForm()
        {
            ///
        }

        private void EquipmentForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            //if (e.CloseReason == CloseReason.UserClosing)
            //{
            //    Application.Exit();
            //}
        }

        private void Panel2_Click(object sender, EventArgs e)
        {
            this.Close();
            DialogResult = DialogResult.OK;
        }
    }
}
