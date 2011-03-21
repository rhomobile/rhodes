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
            DefineGlobalModule("WebView", typeof(rho.rubyext.RhoWebView), 0x00000008, LoadWebView_Instance, LoadWebView_Class, null, IronRuby.Builtins.RubyModule.EmptyArray);
            DefineGlobalClass("Database", typeof(rho.rubyext.RhoDatabase), 0x00000008, classRef0, LoadDatabase_Instance, null, null, new IronRuby.Builtins.RubyModule[] {def1}, 
                new Func<IronRuby.Builtins.RubyClass, IronRuby.Builtins.MutableString, rho.rubyext.RhoDatabase>(rho.rubyext.RhoDatabase.Create)
            );
        }
        
        private static void LoadDatabase_Instance(IronRuby.Builtins.RubyModule/*!*/ module) {
            DefineLibraryMethod(module, "close", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoDatabase, System.Int32>(rho.rubyext.RhoDatabase.Close)
            );
            
            DefineLibraryMethod(module, "commit", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoDatabase, System.Int32>(rho.rubyext.RhoDatabase.Commit)
            );
            
            DefineLibraryMethod(module, "destroy_tables", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoDatabase, System.Int64, System.Int64, System.Int32>(rho.rubyext.RhoDatabase.destroyTables)
            );
            
            DefineLibraryMethod(module, "execute", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoDatabase, IronRuby.Builtins.MutableString, IronRuby.Builtins.RubyArray>(rho.rubyext.RhoDatabase.Execute)
            );
            
            DefineLibraryMethod(module, "initialize", 0x12, 
                0x00000002U, 
                new Func<rho.rubyext.RhoDatabase, IronRuby.Builtins.MutableString, rho.rubyext.RhoDatabase>(rho.rubyext.RhoDatabase.Reinitialize)
            );
            
            DefineLibraryMethod(module, "is_ui_waitfordb", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoDatabase, System.Boolean>(rho.rubyext.RhoDatabase.isUiWaitForDb)
            );
            
            DefineLibraryMethod(module, "lock_db", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoDatabase, System.Int32>(rho.rubyext.RhoDatabase.Lock)
            );
            
            DefineLibraryMethod(module, "rollback", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoDatabase, System.Int32>(rho.rubyext.RhoDatabase.Rollback)
            );
            
            DefineLibraryMethod(module, "start_transaction", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoDatabase, System.Int32>(rho.rubyext.RhoDatabase.startTransaction)
            );
            
            DefineLibraryMethod(module, "table_exist?", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoDatabase, IronRuby.Builtins.MutableString, System.Boolean>(rho.rubyext.RhoDatabase.isTableExist)
            );
            
            DefineLibraryMethod(module, "unlock_db", 0x11, 
                0x00000000U, 
                new Func<rho.rubyext.RhoDatabase, System.Int32>(rho.rubyext.RhoDatabase.Unlock)
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

