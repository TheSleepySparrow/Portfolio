﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Project_AP
{
    public partial class EquipmentListForm : Form
    {
        List<int> checkBoxTag = new();
        public EquipmentListForm()
        {
            InitializeComponent();
            tableLayoutPanel5.BorderStyle = BorderStyle.FixedSingle;
            tableLayoutPanel5.CellBorderStyle = TableLayoutPanelCellBorderStyle.None;
            this.WindowState = FormWindowState.Maximized;
        }

        private void EquipmentListForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                Application.Exit();
            }
        }

        private void EquipmentListForm_Paint(object sender, PaintEventArgs e)
        {
            ControlPaint.DrawBorder(e.Graphics, ClientRectangle, Color.FromArgb(143, 142, 191), ButtonBorderStyle.Solid);
        }

        private async void SearchButton_Click(object sender, EventArgs e)
        {
            string searchQuery = SearchTextBox.Text;

            string apiUrl = "https://helow19274.ru/aip/api/hardware";
            string authorizationToken = "5RNYBdLduTDxQCcM8YYrb5nA:H4dScAyGbS89KgLgZBs2vPsk";

            HardwareService hardwareService = new(apiUrl, authorizationToken);

            try
            {
                List<Hardware> list = await hardwareService.GetHardwareInfoApi(searchQuery);

                HardwareList.Controls.Clear();
                if (list.Count > 0)
                {
                    foreach (Hardware item in list)
                    {
                        int width = (int)(HardwareList.Width * 0.9);
                        int height = (int)(HardwareList.Height * 0.2);
                        Panel AllHardware = new()
                        {
                            BackColor = Color.White,
                            Margin = new Padding(20),
                            Size = new Size(width, height),
                            Location = new Point((HardwareList.Width - width) / 2, height + 10),
                            Anchor = AnchorStyles.None,
                            Dock = DockStyle.None
                        };
                        int checkWidth = (int)(height * 0.8);
                        MyCheckBox checkBox = new()
                        {
                            Dock = DockStyle.Left,
                            Tag = item.id,
                        };
                        AllHardware.Controls.Add(checkBox);
                        checkBox.Click += CheckBox_Click;

                        int buttonWidth = (int)(width * 0.2);
                        Button detailsButton = new()
                        {
                            Text = "Подробнее",
                            FlatStyle = FlatStyle.Flat,
                            Size = new Size(buttonWidth, checkWidth),
                            BackColor = Color.FromArgb(143, 142, 191),
                            Dock = DockStyle.Right,
                            Tag = item.id,
                        };
                        detailsButton.Click += DetailsButton_Click;
                        AllHardware.Controls.Add(detailsButton);

                        Label nameLabel = new()
                        {
                            Text = item.name,
                            Margin = new Padding(10),
                            TextAlign = ContentAlignment.MiddleCenter,
                            //AutoSize = true,
                            Font = new Font("Segoe UI", 18F, FontStyle.Regular, GraphicsUnit.Point),
                            Dock = DockStyle.Fill
                        };
                        AllHardware.Controls.Add(nameLabel);

                        HardwareList.Controls.Add(AllHardware);
                    }
                }
                else
                {
                    int width = (int)(HardwareList.Width * 0.9);
                    int height = (int)(HardwareList.Height * 0.2);
                    int loc = (int)((HardwareList.Width - width) / 2);
                    Panel noHardware = new()
                    {
                        BackColor = Color.White,
                        Padding = new Padding(10),
                        Size = new Size(width, height),
                        Location = new Point(loc, height + 10)
                    };

                    Label noHardwareLabel = new()
                    {
                        Text = "По вашему запросу ничего не найдено",
                        Margin = new Padding(10),
                        TextAlign = ContentAlignment.MiddleCenter,
                        Font = new Font("Segoe UI", 18F, FontStyle.Regular, GraphicsUnit.Point),
                        Dock = DockStyle.Fill
                    };
                    noHardware.Controls.Add(noHardwareLabel);

                    HardwareList.Controls.Add(noHardware);
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show($"An error occurred: {ex.Message}");
            }
        }
        private void SearchTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            SearchButton_Click(sender, e);
        }
        private void DetailsButton_Click(object sender, EventArgs e)
        {
            Button button = (Button)sender;
            int hardware_id = (int)button.Tag;

            EquipmentForm newForm = new()
            {
                Size = this.Size
            };
            this.Hide();
            newForm.Tag = hardware_id;
            newForm.ShowDialog();

            if (newForm.DialogResult == DialogResult.OK)
            {
                this.Show();
            }
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

        private void UsersLabel_Click(object sender, EventArgs e)
        {
            UsersListForm newForm = new()
            {
                Size = this.Size
            };
            this.Hide();
            newForm.ShowDialog();
            this.Close();
        }
        private void StorageLabel_Click(object sender, EventArgs e)
        {
            StorageListForm newForm = new()
            {
                Size = this.Size
            };
            this.Hide();
            newForm.ShowDialog();
            this.Close();
        }
        private void CheckBox_Click(object sender, EventArgs e)
        {
            MyCheckBox cb = (MyCheckBox)sender;
            int loc_id = (int)cb.Tag;
            if (cb.Checked)
            {
                //cb.BackColor = Color.FromArgb(255, 122, 114);
                checkBoxTag.Add(loc_id);
            }
            else
            {
                //cb.BackColor = Color.White;
                checkBoxTag.Remove(loc_id);
            }
        }
        private void DeleteButton_Click(object sender, EventArgs e)
        {
            if (checkBoxTag.Count != 0)
            {
                foreach (int id in checkBoxTag)
                {
                    API api = new();
                    api.DeleteAllHardwareInfo(id);
                }
                checkBoxTag.Clear();
                HardwareList.Controls.Clear();
            }
        }

        private void AddButton_Click(object sender, EventArgs e)
        {
            EquipmentAddForm newForm = new();
            newForm.ShowDialog();
        }
    }
}
