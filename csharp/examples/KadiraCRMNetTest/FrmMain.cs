﻿/*
Copyright (c) 2014, kapsule.code, All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3.0 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library.
*/

using System;
using System.Windows.Forms;
using VtigerWebservice;
using System.Collections.Generic;

/*
 * CODE SAMPLES.
 * 
 * LIST TYPES
 * -----------------------------------------------------------------------------------
 * Dictionary<string, WSClient.ListTypeInfo> list_type = lpConnection.doListTypes();
 * 
 * RETRIVE
 * -----------------------------------------------------------------------------------
 * Dictionary<string, object> item = wsClient.doRetrieve("17x12367");
 * 
 * UPDATE
 * -----------------------------------------------------------------------------------
 * Dictionary<string, object> item_update = new Dictionary<string, object>(item);
   wsClient.doUpdate(item_update);
 * 
 */

namespace KadiraCRM_Net
{
    public partial class FrmMain : Form
    {
        WSClient wsClient = null;

        public FrmMain()
        {
            InitializeComponent();
        }

        private void btnLogin_Click(object sender, EventArgs e)
        {
            try
            { 
                using (FrmUser usr = new FrmUser()){
                    if (usr.ShowDialog(this) == System.Windows.Forms.DialogResult.OK)
                    {
                        tBar.Enabled = false;
                        this.Cursor = Cursors.WaitCursor;

                        this.wsClient = new WSClient(usr.url);
                        
                        if(this.wsClient.doLogin(usr.user_name, usr.key)){
                            lbStatusConnection.Text = "Conectado";
                            lbUserName.Text = usr.user_name;
                            btnLogin.Enabled = false;
                            btnDisconnect.Enabled = true;
                            btnQuery.Enabled = true;
                        }
                    }
                }
            }
            catch(Exception exc)
            {
                MessageBox.Show(this, exc.Message, Application.ProductName,
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally
            {
                tBar.Enabled = true;
                this.Cursor = Cursors.Default;
            }
        }

        private void btnDisconnect_Click(object sender, EventArgs e)
        {
            if (wsClient != null){
                wsClient = null;
                btnLogin.Enabled = true;
                btnDisconnect.Enabled = false;
                btnQuery.Enabled = false;
            }
        }

        private void btnQuery_Click(object sender, EventArgs e)
        {
            try
            {
                if (wsClient != null)
                {
                    this.Cursor = Cursors.WaitCursor;

                    Dictionary<string, object> items = wsClient.doQuery("select * from helpdesk;");

                    if (items.Count >= 1)
                    {
                        lstData.Clear();

                        lstData.Columns.Add("Label");
                        lstData.Columns.Add("Value");

                        foreach (KeyValuePair<string, object> x in items)
                        {
                            var val = x.Value as Dictionary<string, object>;
                            foreach (KeyValuePair<string, object> reg in val)
                            {
                                ListViewItem lvi = lstData.Items.Add(reg.Key);
                                string temp = string.Join(", ", reg.Value);
                                lvi.SubItems.Add(temp);
                            }
                        }
                    }
                }
            }
            catch (Exception exc)
            {
                MessageBox.Show(this, exc.Message, Application.ProductName,
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally
            {
                this.Cursor = Cursors.Default;
            }
        }
    }
}
