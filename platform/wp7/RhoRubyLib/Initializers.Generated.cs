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
            
            
            DefineGlobalModule("WebView", typeof(rho.rubyext.RhoWebView), 0x00000008, LoadWebView_Instance, LoadWebView_Class, null, IronRuby.Builtins.RubyModule.EmptyArray);
        }
        
        private static void LoadWebView_Instance(IronRuby.Builtins.RubyModule/*!*/ module) {
            DefineLibraryMethod(module, "navigate", 0x12, 
                0x00010000U, 
                new Func<System.Object, System.String, System.Boolean>(rho.rubyext.RhoWebView.Navigate)
            );
            
        }
        
        private static void LoadWebView_Class(IronRuby.Builtins.RubyModule/*!*/ module) {
            DefineLibraryMethod(module, "navigate", 0x21, 
                0x00010000U, 
                new Func<System.Object, System.String, System.Boolean>(rho.rubyext.RhoWebView.Navigate)
            );
            
        }
        
    }
}

