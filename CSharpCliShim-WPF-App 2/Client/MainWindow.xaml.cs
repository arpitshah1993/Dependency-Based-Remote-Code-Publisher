///////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - consists client functions            //
// ver 1.0                                                   //
// Arpit Shah, CSE687 - Object Oriented Design, Spring 2017 //
//////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package supports rendering client and cotains event for all the controls.
*
*Public Interface:
*-----------------
*   void readfile() //read the file for string
*    void mockChannel(string text)  // instantiate shim
*   
* Maintenance History:
* --------------------
* ver 1.0 : 07 Feb 2017
* - first release
*/
using System;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Windows;
using System.Windows.Controls;

namespace Client
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        StringBuilder sb = new StringBuilder();
        string currenttab = "";
        const string relative = "Client/TestFilesClient/";
        Shim shim;//= new Shim("localhost:8080", Path.GetFullPath(relative));// = new Shim(Path.GetFullPath(relative));
        string treeXmlq="";
        string dirs;
        Thread tRcv = null;

        /// <summary>
        /// instantiate shim
        /// </summary>
        /// <param name="text"></param>
        void mockChannel(string text)
        {
            Console.WriteLine(Path.GetFullPath(relative));
           shim= new Shim( text,Path.GetFullPath(relative));
        }

        /// <summary>
        /// read the string aput blank value
        /// </summary>
        void readfile()
        {
            treeXmlq = "";
        }

        //main windoe constructor
        public MainWindow()
        {
            InitializeComponent();
            readfile();
            prgrsBarConnect.Visibility = Visibility.Hidden;
            prgrsBar.Visibility = Visibility.Hidden;
            prgrsBarDownload.Visibility = Visibility.Hidden;
            for (int i = 1; i <= 3; i++)
            {
                var tab = tabControl.Items[i] as TabItem;
                tab.IsEnabled = false;
            }
        }

        //start client thread
        private void ClientStarter()
        {
            tRcv = new Thread(
              () =>
              {
                  while (true)
                  {
                      String msg = shim.GetMessage();
                      Action<String> act = new Action<string>(messageReaction);
                      Object[] args = { msg };
                      Dispatcher.Invoke(act, args);
                  }
              }
            );
            tRcv.Start();
        }

        /// <summary>
        /// react on each message started by controls
        /// </summary>
        /// <param name="msg"></param>
        private void messageReaction(string msg)
        {
            if (msg.Contains("File Upload"))
            {
                prgrsBar.Visibility = Visibility.Hidden;
            }
            else if(msg.Contains("Treeview"))
            {
                treeXmlq = msg.Split('|')[1];
                treeXmlq= Regex.Replace(treeXmlq, @"\s+", "");
                treeXmlq=Regex.Replace(treeXmlq, @"\r\n?|\n", "");
                populateTreeView();
                populateCombobox();
            }
            else if (msg.Contains("Server Connect"))
            {
                for (int i = 1; i <= 3; i++)
                {
                    var tab = tabControl.Items[i] as TabItem;
                    tab.IsEnabled = true;
                }
                prgrsBarConnect.Visibility = Visibility.Hidden;
                lblConnectStatus.Content = "Connected";
            }
            else if (msg.Contains("DownLoad File"))
            {
                string fileToOpen = msg.Split('|')[1];
                prgrsBarDownload.Visibility = Visibility.Hidden;
                if (string.IsNullOrEmpty(tbxIIS.Text))
                {
                    fileToOpen = Path.GetFullPath(relative) + fileToOpen + ".htm";
                    if (!File.Exists(fileToOpen))
                        return;
                }
                else
                    fileToOpen = tbxIIS.Text +"/"+ fileToOpen + ".htm";
                System.Diagnostics.Process.Start(fileToOpen);
            }
            else if (msg.Contains("HtmlFileView"))
            {
                listBoxHtmlFiles.Items.Clear();
                string[] filesHtml = msg.Split('|')[1].Split(';');
                for(int i=0; i< filesHtml.Length-1;i++)
                {
                    listBoxHtmlFiles.Items.Add(filesHtml[i]);
                }
            }
        }

        //fire this event on button browse
        private void btnBrowse_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Forms.OpenFileDialog d = new System.Windows.Forms.OpenFileDialog();
            d.Filter = "Code Files|*.cpp;*.h;*.cs";
            d.Multiselect = true;
            if (d.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                foreach (string fileName in d.FileNames)
                {
                    listBox.Items.Add(fileName + ";");
                    sb.Append(fileName + ";");
                }
            }
        }

        //fire this event on button upload
        private void btnUpload_Click(object sender, RoutedEventArgs e)
        {
            string files = sb.ToString();
            if (string.IsNullOrEmpty(files))
                return;
            
            prgrsBar.Visibility = Visibility.Visible;
            prgrsBar.IsIndeterminate = true;
            if(!string.IsNullOrEmpty(comboBox.Text))
            shim.PostMessage("File Upload|" + files+"|"+comboBox.Text);
        }

        //fire this event on button connect
        private void button_Click(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrEmpty(tbxPort.Text))
                return;
            if (string.IsNullOrEmpty(tbxPortClient.Text))
                return;
            mockChannel("localhost:"+tbxPortClient.Text);
            ClientStarter();
            for (int i = 1; i <= 3; i++)
            {
                var tab = tabControl.Items[i] as TabItem;
                tab.IsEnabled = false;
            }
            prgrsBarConnect.Visibility = Visibility.Visible;
            lblConnectStatus.Content = "Connecting..";
            shim.PostMessage("Server Connect|localhost:"+tbxPort.Text);
            shim.PostMessage("Treeview|");
        }

        //populate the Tree view
        public void populateTreeView()
        {
            dirs = "";
            treeView.Items.Clear();
            TreeViewItem repo = new TreeViewItem() { Header = "Repository" };
            if (!string.IsNullOrEmpty(treeXmlq))
            {
                TreeViewData treeViewData = TreeViewData.FromXml(treeXmlq);
                foreach(element elem in treeViewData.elementData)
                {
                    TreeViewItem parent = new TreeViewItem() { Header = elem.parent };
                    dirs += elem.parent + ";";
                    foreach (string child in elem.child)
                    {
                        TreeViewItem childData = new TreeViewItem() { Header = child };
                        parent.Items.Add(childData);
                    }
                    repo.Items.Add(parent);
                }
            }
            treeView.Items.Add(repo);
        }

        //populate the combo box of category list
        private void populateCombobox()
        {
            dirs = dirs.Remove(dirs.Length - 1);
            comboBox.ItemsSource = dirs.Split(';');
        }

        //fire this event on  tabSelection Changed
        private void tabSelection_Changed(object sender, SelectionChangedEventArgs e)
        {
            string tabItem = ((sender as TabControl).SelectedItem as TabItem).Header as string;
            if (currenttab != tabItem)
                currenttab = tabItem;
            else
                return;

            switch (tabItem)
            {
                case "Explorer":
                    shim.PostMessage("Treeview|");
                    break;
                case "Uploader":
                    shim.PostMessage("Treeview|");
                    break;
                case "Delete Html Files":
                    shim.PostMessage("HtmlFileView|");
                    break;
                default:
                    return;
            }
        }

        //fire this event on button download
        private void btnDownload_Click(object sender, RoutedEventArgs e)
        {
            if (treeView.SelectedItem == null)
                return;
            string s = "";
            TreeViewItem item1 = treeView.SelectedItem as TreeViewItem;
            s = item1.Header.ToString();
            if (!s.Contains("."))
                return;
            prgrsBarDownload.Visibility = Visibility.Visible;
            if (treeView.Items.Count >= 0)
            {
                var tree = treeView;
                if (tree != null && tree.SelectedValue != null)
                {
                    TreeViewItem item = tree.SelectedItem as TreeViewItem;
                    s = item.Header.ToString();
                    TreeViewItem parent = item.Parent as TreeViewItem;
                    while (parent != null && parent.GetType() == typeof(TreeViewItem))
                    {
                        s = parent.Header.ToString() +"/"+s;
                        parent = parent.Parent as TreeViewItem;
                    }
                }
            }
            shim.PostMessage("DownLoad File|" + s);
        }

        //fire this event on button clear
        private void btnClear_Click(object sender, RoutedEventArgs e)
        {
            sb.Clear();
            listBox.Items.Clear();
        }

        ////fire this event on button refresh
        private void btnRefresh_Click(object sender, RoutedEventArgs e)
        {
            shim.PostMessage("Treeview|" + sb);
        }

        ////fire this event on button delete
        private void Delete_Click(object sender, RoutedEventArgs e)
        {
            if (treeView.SelectedItem == null)
                return;
            string s = "";
            TreeViewItem item1 = treeView.SelectedItem as TreeViewItem;
            s = item1.Header.ToString();
            if (!s.Contains("."))
                return;
            if (treeView.Items.Count >= 0)
            {
                var tree = treeView;
                if (tree != null && tree.SelectedValue != null)
                {
                    TreeViewItem item = tree.SelectedItem as TreeViewItem;
                    s = item.Header.ToString();
                    TreeViewItem parent = item.Parent as TreeViewItem;
                    while (parent != null && parent.GetType() == typeof(TreeViewItem))
                    {
                        s = parent.Header.ToString() + "/" + s;
                        parent = parent.Parent as TreeViewItem;
                    }
                }
            }
            shim.PostMessage("Delete File|" + s);
        }

        ////fire this event on button deleteHtmlFiles
        private void deleteHtmlFiles_Click_1(object sender, RoutedEventArgs e)
        {
            StringBuilder htmlFiles =new StringBuilder();
            foreach(var item in listBoxHtmlFiles.SelectedItems)
            {
                htmlFiles.Append(item + ";");
            }
            shim.PostMessage("Deletehtml File|" + htmlFiles.ToString());
        }
    }
}
