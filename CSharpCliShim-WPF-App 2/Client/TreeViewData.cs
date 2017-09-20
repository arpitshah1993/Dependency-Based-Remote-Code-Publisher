///////////////////////////////////////////////////////////////
// TreeViewData.cs - Strcture class to store directore view of repo //
// ver 1.0                                                   //
// Arpit Shah, CSE687 - Object Oriented Design, Spring 2017 //
//////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package supports rendering of treeview of directory structure of 
* Server. It has server function to convert the XML strcuture to the class struture
* of directory view.
*
*Public Interface:
*-----------------
*  public void addChild(string name)     //add child
*   public static TreeViewData FromXml(string xml) //Convert XML into Treeview class structure
*   
* Maintenance History:
* --------------------
* ver 1.0 : 07 Feb 2017
* - first release
*/

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace Client
{
    public class element
    {
        public string parent { get; set; }
        public List<string> child { get; set; } = new List<string>();
        public element() { }
        public element(string name)
        {
            parent = name;
        }
        //add child
        public void addChild(string name)
        {
            child.Add(name);
        }
    }
    public class TreeViewData
    {
        public List<element> elementData { get; set; } = new List<element>();

        /// <summary>
        /// Convert XML into Treeview class structure
        /// </summary>
        /// <param name="xml"></param>
        /// <returns></returns>
        public static TreeViewData FromXml(string xml)
        {
            try
            {
                var serializer = new XmlSerializer(typeof(TreeViewData));
                return (TreeViewData)serializer.Deserialize(new StringReader(xml));
            }
            catch (Exception ex)
            {
                Console.Write("\n  deserialization failed\n  {0}", ex.Message);
                return default(TreeViewData);
            }
        }
    }
}


