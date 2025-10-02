#include <stdlib.h>
#include <string>
#include <string.h>
#include <glib.h>

extern "C" {

void rho_sys_app_exit() { exit(0); }

void rho_conf_show_log() { /* no-op for GTK runner */ }

#ifdef RHO_ENABLE_CORE
extern "C" void Init_CoreAPI_Extension();
extern "C" const char* rho_native_rhopath();
#else
extern "C" void Init_CoreAPI_Extension() { /* stub */ }
#endif

#ifdef RHO_ENABLE_CORE
const char* rho_native_reruntimepath() { return rho_native_rhopath(); }
#else
const char* rho_native_reruntimepath() { return ""; }
#endif

}

#ifdef RHO_ENABLE_CORE

#include <gtk/gtk.h>
#ifdef WEBKITGTK6
#include <webkit/webkit.h>
#else
#include <webkit2/webkit2.h>
#endif

#include "logging/RhoLog.h"
#include "common/RhodesApp.h"
#include "ruby/ext/rho/rhoruby.h"
#include "../../../shared/api_generator/js_helpers.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "GtkWebView"

namespace {

struct IdleUriRequest {
    gchar *uri;
};

struct IdleJsRequest {
    gchar *script;
};

static GMutex s_view_mutex;
static WebKitWebView *s_main_view = nullptr;
static GtkWindow *s_main_window = nullptr;
static std::string s_current_url;

static const char kNativePromptTag[] = "__rhoNativeApiCall:prompt";
static const char kBridgeInstalledKey[] = "rho-native-bridge-installed";
static const size_t kBridgeLogLimit = 256;
static const char kBridgeScript[] = R"JS(
(function() {
  var global = window;
  if (!global.__rhoNativeApi) {
    global.__rhoNativeApi = {};
  }
  if (typeof global.__rhoNativeApi.apiCall !== 'function') {
    global.__rhoNativeApi.apiCall = function(cmdText, async) {
      try {
        return global.prompt(cmdText, '__rhoNativeApiCall:prompt');
      } catch (e) {
        return JSON.stringify({ error: { message: e && e.message ? e.message : 'prompt bridge failure' } });
      }
    };
  }
  if (typeof global.__rhoNativeApi.toLowerCase !== 'function') {
    global.__rhoNativeApi.toLowerCase = function(value) {
      return String(value).toLowerCase();
    };
  }
  try {
    global.__rho_nativeBridgeType = 'win32';
  } catch (ignored) {
  }
})();
)JS";

static gboolean with_view(gpointer data, gboolean (*cb)(WebKitWebView*, gpointer)) {
    gboolean result = G_SOURCE_REMOVE;
    g_mutex_lock(&s_view_mutex);
    WebKitWebView *view = s_main_view;
    g_mutex_unlock(&s_view_mutex);
    if (view) {
        result = cb(view, data);
    }
    return result;
}

static gboolean idle_load_uri(WebKitWebView *view, gpointer data) {
    IdleUriRequest *req = static_cast<IdleUriRequest*>(data);
    if (req && req->uri) {
        g_message("[GtkWebView] idle_load_uri dispatch uri=%s", req->uri);
        webkit_web_view_load_uri(view, req->uri);
    }
    if (req) {
        g_free(req->uri);
        g_free(req);
    }
    return G_SOURCE_REMOVE;
}

static gboolean idle_reload(WebKitWebView *view, gpointer data) {
    (void)data;
    webkit_web_view_reload(view);
    return G_SOURCE_REMOVE;
}

static gboolean idle_go_back(WebKitWebView *view, gpointer data) {
    (void)data;
    if (webkit_web_view_can_go_back(view))
        webkit_web_view_go_back(view);
    return G_SOURCE_REMOVE;
}

static gboolean idle_go_forward(WebKitWebView *view, gpointer data) {
    (void)data;
    if (webkit_web_view_can_go_forward(view))
        webkit_web_view_go_forward(view);
    return G_SOURCE_REMOVE;
}

static gboolean idle_run_js(WebKitWebView *view, gpointer data) {
    IdleJsRequest *req = static_cast<IdleJsRequest*>(data);
    if (req && req->script) {
        g_message("[GtkWebView] idle_run_js len=%zu", strlen(req->script));
#ifdef WEBKITGTK6
        webkit_web_view_evaluate_javascript(view, req->script, -1, nullptr, nullptr, nullptr, nullptr, nullptr);
#else
        webkit_web_view_run_javascript(view, req->script, nullptr, nullptr, nullptr);
#endif
    }
    if (req) {
        g_free(req->script);
        g_free(req);
    }
    return G_SOURCE_REMOVE;
}

} // namespace

extern "C" {

static void ensure_bridge_script(WebKitWebView *view) {
    if (!view)
        return;

    gpointer installed = g_object_get_data(G_OBJECT(view), kBridgeInstalledKey);
    if (installed == GINT_TO_POINTER(1))
        return;

    WebKitUserContentManager *manager = webkit_web_view_get_user_content_manager(view);
    if (!manager)
        return;

    WebKitUserScript *script = webkit_user_script_new(
        kBridgeScript,
        WEBKIT_USER_CONTENT_INJECT_TOP_FRAME,
        WEBKIT_USER_SCRIPT_INJECT_AT_DOCUMENT_START,
        nullptr,
        nullptr);

    if (script) {
        webkit_user_content_manager_add_script(manager, script);
        webkit_user_script_unref(script);
        g_object_set_data(G_OBJECT(view), kBridgeInstalledKey, GINT_TO_POINTER(1));
        g_message("[GtkWebView] Native JS bridge script injected");
    }
}

void rho_gtk_register_webview(WebKitWebView *view) {
    g_mutex_lock(&s_view_mutex);
    s_main_view = view;
    g_mutex_unlock(&s_view_mutex);
    ensure_bridge_script(view);
}

void rho_gtk_register_window(GtkWindow *window) {
    g_mutex_lock(&s_view_mutex);
    s_main_window = window;
    g_mutex_unlock(&s_view_mutex);
}

void rho_gtk_update_current_url(const gchar *uri) {
    g_mutex_lock(&s_view_mutex);
    s_current_url = uri ? uri : "";
    g_mutex_unlock(&s_view_mutex);
    g_message("[GtkWebView] update_current_url=%s", uri ? uri : "(null)");
}

void rho_webview_refresh(int index) {
    (void)index;
    g_idle_add_full(G_PRIORITY_DEFAULT, [](gpointer data) -> gboolean {
        return with_view(data, idle_reload);
    }, nullptr, nullptr);
}

void rho_webview_full_screen_mode(int enable) {
    (void)enable;
}

int rho_webview_active_tab() {
    return 0;
}

const char* rho_webview_execute_js(const char* js, int index) {
    (void)index;
    if (!js) {
        fprintf(stderr, "[GtkWebView] rho_webview_execute_js(%d) received null script\n", index);
        fflush(stderr);
        return "";
    }
    g_message("[GtkWebView] rho_webview_execute_js(%d) len=%zu", index, strlen(js));
    IdleJsRequest *req = static_cast<IdleJsRequest*>(g_malloc0(sizeof(IdleJsRequest)));
    req->script = g_strdup(js);
    g_idle_add_full(G_PRIORITY_DEFAULT, [](gpointer data) -> gboolean {
        return with_view(data, idle_run_js);
    }, req, nullptr);
    return "";
}

const char* rho_webview_execute_js_sync(const char* js, int index) {
    return rho_webview_execute_js(js, index);
}

const char* rho_webview_current_location(int index) {
    (void)index;
    g_mutex_lock(&s_view_mutex);
    const char *ret = s_current_url.c_str();
    g_mutex_unlock(&s_view_mutex);
    return ret;
}

void rho_webview_navigate(const char* url, int index) {
    (void)index;
    if (!url) return;
    std::string finalUrl = RHODESAPP().canonicalizeRhoUrl(url);
    g_message("[GtkWebView] rho_webview_navigate(%d) request url=%s final=%s", index, url, finalUrl.c_str());
    IdleUriRequest *req = static_cast<IdleUriRequest*>(g_malloc0(sizeof(IdleUriRequest)));
    req->uri = g_strdup(finalUrl.c_str());
    g_idle_add_full(G_PRIORITY_DEFAULT, [](gpointer data) -> gboolean {
        return with_view(data, idle_load_uri);
    }, req, nullptr);
}

void rho_webview_navigate_back() {
    g_idle_add_full(G_PRIORITY_DEFAULT, [](gpointer data) -> gboolean {
        return with_view(data, idle_go_back);
    }, nullptr, nullptr);
}

void rho_webview_navigate_back_with_tab(int index) {
    (void)index;
    rho_webview_navigate_back();
}

void rho_webview_navigate_forward() {
    g_idle_add_full(G_PRIORITY_DEFAULT, [](gpointer data) -> gboolean {
        return with_view(data, idle_go_forward);
    }, nullptr, nullptr);
}

void alert_show_status(const char* szTitle, const char* szMessage, const char* szHide) {
    (void)szHide;
    const gchar *title = szTitle ? szTitle : "Rhodes";
    const gchar *message = szMessage ? szMessage : "";
    g_message("[%s] %s", title, message);
}

gboolean rho_gtk_handle_script_dialog(WebKitWebView *view, WebKitScriptDialog *dialog) {
    if (!dialog)
        return FALSE;

    if (webkit_script_dialog_get_dialog_type(dialog) != WEBKIT_SCRIPT_DIALOG_PROMPT)
        return FALSE;

    const gchar *default_text = webkit_script_dialog_prompt_get_default_text(dialog);
    if (!default_text || g_strcmp0(default_text, kNativePromptTag) != 0)
        return FALSE;

    const gchar *message = webkit_script_dialog_get_message(dialog);
    const char *payload = message ? message : "";

    size_t payload_len = strlen(payload);
    g_autofree gchar *payload_preview = g_strndup(payload, MIN(payload_len, kBridgeLogLimit));
    g_message("[GtkWebView] Handling __rhoNativeApiCall payload length=%zu preview=%s%s",
              payload_len,
              payload_preview ? payload_preview : "(null)",
              payload_len > kBridgeLogLimit ? "…" : "");

    rho::String result;
    try {
        result = rho::apiGenerator::js_entry_point(payload);
    } catch (...) {
        RAWLOG_ERROR("js_entry_point threw while handling native API call");
        result = "{\"error\":{\"message\":\"native exception\"}}";
    }

    webkit_script_dialog_prompt_set_text(dialog, result.c_str());
    size_t result_len = result.length();
    g_autofree gchar *result_preview = g_strndup(result.c_str(), MIN(result_len, kBridgeLogLimit));
    g_message("[GtkWebView] __rhoNativeApiCall responded with %zu bytes preview=%s%s",
              result_len,
              result_preview ? result_preview : "(null)",
              result_len > kBridgeLogLimit ? "…" : "");
    return TRUE;
}

void rho_gtk_install_native_bridge(WebKitWebView *view) {
    ensure_bridge_script(view);
}

} // extern "C"

#else  // !RHO_ENABLE_CORE

extern "C" {

void rho_webview_refresh(int index) { (void)index; }
void rho_webview_full_screen_mode(int enable) { (void)enable; }
int  rho_webview_active_tab() { return 0; }
const char* rho_webview_execute_js(const char* js, int index) { (void)js; (void)index; return ""; }
const char* rho_webview_execute_js_sync(const char* js, int index) { (void)js; (void)index; return ""; }
const char* rho_webview_current_location(int index) { (void)index; return ""; }
void rho_webview_navigate(const char* url, int index) { (void)url; (void)index; }
void rho_webview_navigate_back() {}
void rho_webview_navigate_back_with_tab(int index) { (void)index; }
void rho_webview_navigate_forward() {}
void alert_show_status(const char* szTitle, const char* szMessage, const char* szHide) {
    (void)szTitle; (void)szMessage; (void)szHide;
}

gboolean rho_gtk_handle_script_dialog(void* /*view*/, void* /*dialog*/) { return FALSE; }
void rho_gtk_install_native_bridge(void* /*view*/) {}

}

#endif // RHO_ENABLE_CORE
