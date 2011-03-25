/* ****************************************************************************
 *
 * Copyright (c) Microsoft Corporation. 
 *
 * This source code is subject to terms and conditions of the Apache License, Version 2.0. A 
 * copy of the license can be found in the License.html file at the root of this distribution. If 
 * you cannot locate the  Apache License, Version 2.0, please send an email to 
 * ironruby@microsoft.com. By using this source code in any fashion, you are agreeing to be bound 
 * by the terms of the Apache License, Version 2.0.
 *
 * You must not remove this notice, or any other, from this software.
 *
 *
 * ***************************************************************************/

#pragma warning disable 169 // mcs: unused private method
[assembly: IronRuby.Runtime.RubyLibraryAttribute(typeof(rho.rubyext.rubyextLibraryInitializer))]

namespace rho.rubyext {
    using System;
    using Microsoft.Scripting.Utils;
    using System.Runtime.InteropServices;
    
    public sealed class rubyextLibraryInitializer : IronRuby.Builtins.LibraryInitializer {
        protected override void LoadModules() {
            IronRuby.Builtins.RubyClass classRef0 = GetClass(typeof(System.Object));
            
            
            IronRuby.Builtins.RubyModule def1 = DefineGlobalModule("SQLite3", typeof(rho.rubyext.RhoSQLite3), 0x00000008, null, null, null, IronRuby.Builtins.RubyModule.EmptyArray);
            IronRuby.Builtins.RubyClass def2 = DefineClass("SQLite3::Database", typeof(rho.rubyext.RhoSQLite3.RhoDatabase), 0x00000008, classRef0, LoadSQLite3__Database_Instance, null, null, IronRuby.Builtins.RubyModule.EmptyArray, 
                new Func<IronRuby.Builtins.RubyClass, IronRuby.Builtins.MutableString, IronRuby.Builtins.MutableString, rho.rubyext.RhoSQLite3.RhoDatabase>(rho.rubyext.RhoSQLite3.RhoDatabase.Create)
            );
            DefineGlobalModule("SyncEngine", typeof(rho.rubyext.RhoSyncEngine), 0x00000008, null, LoadSyncEngine_Class, null, IronRuby.Builtins.RubyModule.EmptyArray);
            DefineGlobalModule("WebView", typeof(rho.rubyext.RhoWebView), 0x00000008, LoadWebView_Instance, LoadWebView_Class, null, IronRuby.Builtins.RubyModule.EmptyArray);
            SetConstant(def1, "Database", def2);
        }
        
        private static void LoadSQLite3__Database_Instance(IronRuby.Builtins.RubyModule/*!*/ module) {
            DefineLibraryMethod(module, "close", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoSQLite3.RhoDatabase, System.Int32>(rho.rubyext.RhoSQLite3.RhoDatabase.Close)
            );
            
            DefineLibraryMethod(module, "commit", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoSQLite3.RhoDatabase, System.Int32>(rho.rubyext.RhoSQLite3.RhoDatabase.Commit)
            );
            
            DefineLibraryMethod(module, "destroy_tables", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoSQLite3.RhoDatabase, System.Int64, System.Int64, System.Int32>(rho.rubyext.RhoSQLite3.RhoDatabase.destroyTables)
            );
            
            DefineLibraryMethod(module, "execute", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoSQLite3.RhoDatabase, IronRuby.Builtins.MutableString, System.Boolean, IronRuby.Builtins.RubyArray, IronRuby.Builtins.RubyArray>(rho.rubyext.RhoSQLite3.RhoDatabase.Execute)
            );
            
            DefineLibraryMethod(module, "is_ui_waitfordb", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoSQLite3.RhoDatabase, System.Boolean>(rho.rubyext.RhoSQLite3.RhoDatabase.isUiWaitForDb)
            );
            
            DefineLibraryMethod(module, "lock_db", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoSQLite3.RhoDatabase, System.Int32>(rho.rubyext.RhoSQLite3.RhoDatabase.Lock)
            );
            
            DefineLibraryMethod(module, "rollback", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoSQLite3.RhoDatabase, System.Int32>(rho.rubyext.RhoSQLite3.RhoDatabase.Rollback)
            );
            
            DefineLibraryMethod(module, "start_transaction", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoSQLite3.RhoDatabase, System.Int32>(rho.rubyext.RhoSQLite3.RhoDatabase.startTransaction)
            );
            
            DefineLibraryMethod(module, "table_exist?", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoSQLite3.RhoDatabase, IronRuby.Builtins.MutableString, System.Boolean>(rho.rubyext.RhoSQLite3.RhoDatabase.isTableExist)
            );
            
            DefineLibraryMethod(module, "unlock_db", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoSQLite3.RhoDatabase, System.Int32>(rho.rubyext.RhoSQLite3.RhoDatabase.Unlock)
            );
            
        }
        
        private static void LoadSyncEngine_Class(IronRuby.Builtins.RubyModule/*!*/ module) {
            DefineLibraryMethod(module, "dosync", 0x21, 
                0x00000000U, 0x00000000U, 
                new Func<IronRuby.Builtins.RubyModule, System.Object>(rho.rubyext.RhoSyncEngine.dosync), 
                new Func<IronRuby.Builtins.RubyModule, System.Boolean, System.Object>(rho.rubyext.RhoSyncEngine.dosync)
            );
            
            DefineLibraryMethod(module, "dosync_source", 0x21, 
                0x00000002U, 0x00000002U, 
                new Func<IronRuby.Builtins.RubyModule, System.Object, System.Object>(rho.rubyext.RhoSyncEngine.dosync_source), 
                new Func<IronRuby.Builtins.RubyModule, System.Object, System.Boolean, System.Object>(rho.rubyext.RhoSyncEngine.dosync_source)
            );
            
            DefineLibraryMethod(module, "enable_status_popup", 0x21, 
                0x00000000U, 
                new Action<IronRuby.Builtins.RubyModule, System.Boolean>(rho.rubyext.RhoSyncEngine.enable_status_popup)
            );
            
            DefineLibraryMethod(module, "logged_in", 0x21, 
                0x00000000U, 
                new Func<IronRuby.Builtins.RubyModule, System.Int32>(rho.rubyext.RhoSyncEngine.logged_in)
            );
            
            DefineLibraryMethod(module, "login", 0x21, 
                0x0000000eU, 
                new Func<IronRuby.Builtins.RubyModule, System.String, System.String, System.String, System.Object>(rho.rubyext.RhoSyncEngine.login)
            );
            
            DefineLibraryMethod(module, "set_notification", 0x21, 
                0x00000006U, 
                new Action<IronRuby.Builtins.RubyModule, System.Int32, System.String, System.String>(rho.rubyext.RhoSyncEngine.set_notification)
            );
            
            DefineLibraryMethod(module, "set_objectnotify_url", 0x21, 
                0x00000002U, 
                new Action<IronRuby.Builtins.RubyModule, System.String>(rho.rubyext.RhoSyncEngine.set_objectnotify_url)
            );
            
            DefineLibraryMethod(module, "update_blob_attribs", 0x21, 
                0x00000006U, 
                new Action<IronRuby.Builtins.RubyModule, System.String, System.Int32>(rho.rubyext.RhoSyncEngine.update_blob_attribs)
            );
            
        }
        
        private static void LoadWebView_Instance(IronRuby.Builtins.RubyModule/*!*/ module) {
            DefineLibraryMethod(module, "navigate", 0x12, 
                0x00010000U, 
                new Action<System.Object, System.String>(rho.rubyext.RhoWebView.Navigate)
            );
            
        }
        
        private static void LoadWebView_Class(IronRuby.Builtins.RubyModule/*!*/ module) {
            DefineLibraryMethod(module, "navigate", 0x21, 
                0x00010000U, 
                new Action<System.Object, System.String>(rho.rubyext.RhoWebView.Navigate)
            );
            
        }
        
    }
}

