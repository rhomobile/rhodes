// GTK AltMobile runner entry point: sets up window chrome, splash, WebKit view,
// and optional Rhodes core integration.
#include <adwaita.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#ifndef NO_WEBKIT
  #ifdef WEBKITGTK6
    #include <webkit/webkit.h>
  #else
    #include <webkit2/webkit2.h>
  #endif
#endif
#include <glib.h>
#include <glib/gfileutils.h>
#include "generated_app_info.h"

void rho_gtk_register_menu_widgets(GtkWidget *button, GtkWidget *box);
void rho_symimpl_createMenu(void);
#if defined(RHO_ENABLE_CORE) && !defined(NO_WEBKIT)
gboolean rho_gtk_handle_script_dialog(WebKitWebView *view, WebKitScriptDialog *dialog);
void rho_gtk_install_native_bridge(WebKitWebView *view);
#endif
#ifndef ALTGTK_APP_NAME
#define ALTGTK_APP_NAME "Rhodes"
#endif
#ifndef ALTGTK_APP_VERSION
#define ALTGTK_APP_VERSION "1.0"
#endif
#ifndef ALTGTK_VENDOR
#define ALTGTK_VENDOR "Tau Technologies"
#endif
#ifndef ALTGTK_APP_ID
#define ALTGTK_APP_ID "com.rhomobile.Rhodes"
#endif
#ifndef ALTGTK_SDK_VERSION
#define ALTGTK_SDK_VERSION ""
#endif
#define ALTGTK_LOG(fmt, ...) \
    do { printf("[AltGtk] " fmt "\n", ##__VA_ARGS__); fflush(stdout); } while (0)

// Utility helpers -----------------------------------------------------------
static gchar *altgtk_derive_app_name(int argc, char **argv);
static GtkWidget *altgtk_create_header(GtkWidget **menu_button_out, GtkWidget **menu_box_out);
static gchar *altgtk_locate_path(const char *app_name, const char *relative_path, GFileTest test);
static gchar *altgtk_path_to_uri(const gchar *path);
#ifdef RHO_ENABLE_CORE
static void altgtk_clear_ruby_environment(void);
static gboolean altgtk_initialize_core(const char *app_name);
#endif

#ifdef RHO_ENABLE_CORE
#include "common/RhoConf.h"
#include "common/RhodesApp.h"
// Provide rho_native_rhopath() for core to locate RhoBundle
static gchar *g_rho_root = NULL;
const char* rho_native_rhopath(){
    ALTGTK_LOG("rho_native_rhopath requested, root=%s", g_rho_root ? g_rho_root : "(unset)");
    return g_rho_root ? g_rho_root : "";
}
static gboolean g_rho_core_started = FALSE;
static gboolean g_rho_core_navigation_failed = FALSE;

void rho_gtk_register_webview(WebKitWebView *view);
void rho_gtk_update_current_url(const gchar *uri);
void rho_gtk_register_window(GtkWindow *window);
#define ALTGTK_CORE_STATE_STRING "enabled"
#else
#define ALTGTK_CORE_STATE_STRING "disabled"
#endif

#ifndef NO_WEBKIT
// Tracks WebView pointer and bundle name for delayed navigation tasks.
typedef struct {
    WebKitWebView *view;
    gchar *app_name;
} LoadCtx;

static gboolean try_load_index(gpointer data);

// Global widget pointers used to drive the splash overlay from multiple callbacks.
// They let asynchronous handlers agree on which child (splash vs. WebView) is visible.
static GtkStack *g_content_stack = NULL;
static GtkWidget *g_webview_widget = NULL;
static GtkWidget *g_splash_widget = NULL;
// Tracks whether the splash overlay is still visible to avoid duplicate transitions.
static gboolean g_splash_visible = FALSE;
// Remembers the initial loading.html URI so we do not hide the splash during subsequent reloads of that page.
static gchar *g_loading_html_uri = NULL;

// Derive application display name from argv[0] or fall back to compile-time defaults.
static gchar *altgtk_derive_app_name(int argc, char **argv) {
    if (argc > 0 && argv && argv[0]) {
        gchar *base = g_path_get_basename(argv[0]);
        if (base && *base) {
            gchar *derived = g_strdup(base);
            g_free(base);
            return derived;
        }
        g_clear_pointer(&base, g_free);
    }
    return g_strdup(ALTGTK_APP_NAME);
}

// Build header bar with menu button on the left and a single close control on the right.
static GtkWidget *altgtk_create_header(GtkWidget **menu_button_out, GtkWidget **menu_box_out) {
    GtkWidget *header = adw_header_bar_new();
    gtk_widget_add_css_class(header, "flat");
    gtk_header_bar_set_decoration_layout(GTK_HEADER_BAR(header), ":close");
    adw_header_bar_set_show_start_title_buttons(ADW_HEADER_BAR(header), FALSE);
    adw_header_bar_set_show_end_title_buttons(ADW_HEADER_BAR(header), FALSE);

    GtkWidget *window_controls = gtk_window_controls_new(GTK_PACK_END);
    gtk_window_controls_set_decoration_layout(GTK_WINDOW_CONTROLS(window_controls), "close");
    adw_header_bar_pack_end(ADW_HEADER_BAR(header), window_controls);

    GtkWidget *menu_button = gtk_menu_button_new();
    gtk_widget_set_visible(menu_button, FALSE);
    gtk_widget_add_css_class(menu_button, "flat");
    gtk_widget_set_valign(menu_button, GTK_ALIGN_CENTER);
    gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(menu_button), "open-menu-symbolic");
    gtk_widget_set_tooltip_text(menu_button, "Menu");
    adw_header_bar_pack_start(ADW_HEADER_BAR(header), menu_button);

    GtkWidget *menu_popover = gtk_popover_new();
    gtk_popover_set_has_arrow(GTK_POPOVER(menu_popover), TRUE);
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(menu_button), menu_popover);

    GtkWidget *menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_widget_set_margin_top(menu_box, 6);
    gtk_widget_set_margin_bottom(menu_box, 6);
    gtk_widget_set_margin_start(menu_box, 6);
    gtk_widget_set_margin_end(menu_box, 6);
    gtk_popover_set_child(GTK_POPOVER(menu_popover), menu_box);

    if (menu_button_out) *menu_button_out = menu_button;
    if (menu_box_out) *menu_box_out = menu_box;

    return header;
}

// Locate a file or directory in typical AltMobile bundle locations (opt, alongside binary, build output).
static gchar *altgtk_locate_path(const char *app_name, const char *relative_path, GFileTest test) {
    if (!relative_path || !*relative_path)
        return NULL;

    gchar *candidate = NULL;
    if (app_name && *app_name) {
        candidate = g_build_filename("/opt", app_name, relative_path, NULL);
        if (candidate && g_file_test(candidate, test))
            return candidate;
        g_clear_pointer(&candidate, g_free);
    }

    gchar *exe_link = g_file_read_link("/proc/self/exe", NULL);
    gchar *exe_dir = exe_link ? g_path_get_dirname(exe_link) : NULL;
    if (exe_dir) {
        candidate = g_build_filename(exe_dir, relative_path, NULL);
        if (candidate && g_file_test(candidate, test)) {
            g_clear_pointer(&exe_dir, g_free);
            g_clear_pointer(&exe_link, g_free);
            return candidate;
        }
        g_clear_pointer(&candidate, g_free);
    }
    g_clear_pointer(&exe_dir, g_free);
    g_clear_pointer(&exe_link, g_free);

    gchar *cwd = g_get_current_dir();
    if (cwd) {
        candidate = g_build_filename(cwd, "bin", "target", "altMobile", relative_path, NULL);
        if (candidate && g_file_test(candidate, test)) {
            g_clear_pointer(&cwd, g_free);
            return candidate;
        }
        g_clear_pointer(&candidate, g_free);
    }
    g_clear_pointer(&cwd, g_free);

    return NULL;
}

// Convert file system path to file:// URI, returning NULL on failure.
static gchar *altgtk_path_to_uri(const gchar *path) {
    if (!path || !*path)
        return NULL;
    return g_filename_to_uri(path, NULL, NULL);
}

#ifdef RHO_ENABLE_CORE
// Remove Ruby-related environment variables that may interfere with embedded VM startup.
static void altgtk_clear_ruby_environment(void) {
    static const char *ruby_env_vars[] = {
        "RUBYLIB", "RUBYOPT", "RUBY_VERSION", "RUBYLIB_PREFIX",
        "GEM_HOME", "GEM_PATH", "GEM_SPEC_CACHE", "MY_RUBY_HOME",
        "BUNDLE_GEMFILE", "BUNDLE_PATH", "BUNDLE_BIN", NULL
    };

    for (int i = 0; ruby_env_vars[i] != NULL; ++i) {
        ALTGTK_LOG("Clearing environment variable %s", ruby_env_vars[i]);
        g_unsetenv(ruby_env_vars[i]);
    }
}

// Initialize Rhodes core subsystems if a bundle directory is available.
static gboolean altgtk_initialize_core(const char *app_name) {
    gchar *rho_root_candidate = altgtk_locate_path(app_name, "rho", G_FILE_TEST_IS_DIR);
    if (!rho_root_candidate) {
        ALTGTK_LOG("Rho root not found; running without core integration");
        return FALSE;
    }

    size_t len = strlen(rho_root_candidate);
    gboolean needs_slash = (len > 0 && rho_root_candidate[len - 1] != '/');
    g_rho_root = needs_slash ? g_strconcat(rho_root_candidate, "/", NULL) : g_strdup(rho_root_candidate);
    g_free(rho_root_candidate);

    ALTGTK_LOG("Initializing Rhodes core with root %s", g_rho_root);
    rho_conf_Init_with_separate_user_path(g_rho_root, g_rho_root);
    ALTGTK_LOG("Configuration initialized");
    rho_rhodesapp_create_with_separate_user_path(g_rho_root, g_rho_root);
    ALTGTK_LOG("Rhodes app created with user path %s", g_rho_root);
    ALTGTK_LOG("Starting core services");
    rho_rhodesapp_start();
    ALTGTK_LOG("Core services started");

    g_rho_core_started = TRUE;
    g_rho_core_navigation_failed = FALSE;
    return TRUE;
}
#endif

// Hide the splash overlay and switch focus back to the main WebView surface.
// Invoked when WebKit commits real navigation or signals an error (WebKitGTK 4.x).
static void altgtk_hide_splash(const gchar *uri_for_log) {
    if (!g_splash_visible)
        return;

    ALTGTK_LOG("Hiding splash overlay (uri=%s)", uri_for_log ? uri_for_log : "(null)");
    g_splash_visible = FALSE;

    if (g_content_stack && GTK_IS_STACK(g_content_stack) && g_webview_widget) {
        gtk_stack_set_visible_child(g_content_stack, g_webview_widget);
    }

    if (g_splash_widget)
        gtk_widget_set_visible(g_splash_widget, FALSE);

    g_splash_widget = NULL;
    g_clear_pointer(&g_loading_html_uri, g_free);
}

#ifdef RHO_ENABLE_CORE
// Polling helper used to keep probing rho_rhodesapp_getstarturl until the HTTP server is ready.
typedef struct {
    WebKitWebView *view;
    gchar *app_name;
    int attempts;
} CoreLoadCtx;

static gboolean try_load_core(gpointer data) {
    CoreLoadCtx *ctx = (CoreLoadCtx*)data;
    ALTGTK_LOG("try_load_core invoked (ctx=%p)", (void*)ctx);
    if (!ctx || !ctx->view) {
        ALTGTK_LOG("try_load_core missing view, cleaning up");
        if (ctx) {
            if (ctx->app_name) g_free(ctx->app_name);
            g_free(ctx);
        }
        return G_SOURCE_REMOVE;
    }

    const char *start_url = rho_rhodesapp_getstarturl();
    ALTGTK_LOG("try_load_core attempt %d, start_url=%s", ctx->attempts, start_url ? start_url : "(null)");
    if (start_url && *start_url) {
        ALTGTK_LOG("try_load_core loading start URL");
        webkit_web_view_load_uri(ctx->view, start_url);
        if (ctx->app_name) g_free(ctx->app_name);
        g_free(ctx);
        return G_SOURCE_REMOVE;
    }

    if (ctx->attempts++ >= 50) {
        if (!g_rho_core_navigation_failed) {
            g_rho_core_navigation_failed = TRUE;
            ALTGTK_LOG("try_load_core exceeded attempts, scheduling index fallback");
            if (ctx->app_name) {
                LoadCtx *fallback = g_new0(LoadCtx, 1);
                fallback->view = ctx->view;
                fallback->app_name = ctx->app_name; // ownership moves to fallback
                ctx->app_name = NULL;
                g_timeout_add (0, try_load_index, fallback);
            }
        }
        ctx->attempts = 0;
        return G_SOURCE_CONTINUE;
    }

    return G_SOURCE_CONTINUE;
}
#endif

// Deferred attempt to open index.html once the splash delay has elapsed.
static gboolean try_load_index(gpointer data) {
    LoadCtx *ctx = (LoadCtx*)data;
    ALTGTK_LOG("try_load_index invoked (ctx=%p)", (void*)ctx);
    if (!ctx || !ctx->view) return G_SOURCE_REMOVE;
#ifdef RHO_ENABLE_CORE
    if (g_rho_core_started && !g_rho_core_navigation_failed) {
        ALTGTK_LOG("Core navigation succeeded, canceling try_load_index");
        if (ctx->app_name) g_free(ctx->app_name);
        g_free(ctx);
        return G_SOURCE_REMOVE;
    }
#endif
    gchar *index_path = altgtk_locate_path(ctx->app_name, "rho/apps/app/index.html", G_FILE_TEST_EXISTS);
    if (!index_path)
        index_path = altgtk_locate_path(ctx->app_name, "rho/apps/index.html", G_FILE_TEST_EXISTS);

    ALTGTK_LOG("try_load_index resolved path=%s", index_path ? index_path : "(none)");

    if (index_path) {
        gchar *uri = altgtk_path_to_uri(index_path);
        if (uri && *uri) {
            ALTGTK_LOG("try_load_index loading %s", uri);
            webkit_web_view_load_uri(ctx->view, uri);
        }
        g_clear_pointer(&uri, g_free);
        g_clear_pointer(&index_path, g_free);
    }

    if (ctx->app_name) g_free(ctx->app_name);
    g_free(ctx);
    return G_SOURCE_REMOVE;
}
// WebKit load-change hook: hides the splash when navigation moves past loading.html and forwards URLs to core.
static void gtk_on_load_changed(WebKitWebView *web_view, WebKitLoadEvent load_event, gpointer user_data) {
    (void)user_data;
    const gchar *uri = webkit_web_view_get_uri(web_view);
    ALTGTK_LOG("gtk_on_load_changed event=%d uri=%s", load_event, uri ? uri : "(null)");
#ifdef RHO_ENABLE_CORE
    rho_gtk_update_current_url(uri);
    if (load_event == WEBKIT_LOAD_FINISHED && uri && g_rho_core_started) {
        ALTGTK_LOG("gtk_on_load_changed load finished, clearing navigation failure flag");
        g_rho_core_navigation_failed = FALSE;
    }
#endif

    if (g_splash_visible) {
        // Keep the splash visible while the WebView reloads the original loading.html resource.
        gboolean keep_splash = FALSE;
        if (uri && g_loading_html_uri && g_strcmp0(uri, g_loading_html_uri) == 0)
            keep_splash = TRUE;

        // WebKitGTK6 never emits WEBKIT_LOAD_FAILED, so we rely on COMMITTED/FINISHED events there.
        gboolean should_hide = (!keep_splash &&
                                (load_event == WEBKIT_LOAD_COMMITTED ||
                                 load_event == WEBKIT_LOAD_FINISHED));
#ifndef WEBKITGTK6
        // WebKitGTK 4.1 (GTK3) does emit WEBKIT_LOAD_FAILED; use it to prevent hanging on the splash.
        if (!should_hide && !keep_splash && load_event == WEBKIT_LOAD_FAILED)
            should_hide = TRUE;
#endif
        if (should_hide)
            altgtk_hide_splash(uri);
    }
}
#ifdef RHO_ENABLE_CORE
static gboolean gtk_on_script_dialog(WebKitWebView *web_view, WebKitScriptDialog *dialog, gpointer user_data) {
    (void)user_data;
    return rho_gtk_handle_script_dialog(web_view, dialog);
}
#endif
#endif

static void on_activate_data_free(gpointer data, GClosure *closure) {
    (void)closure;
    if (data) g_free(data);
}

// Activation handler: constructs the GTK window, header bar, splash stack, and optional core glue.
static void
on_activate (GApplication *app, gpointer user_data)
{
    ALTGTK_LOG("on_activate invoked (app=%p)", (void*)app);
    const char *app_name = (const char *)user_data;

    AdwApplicationWindow *win = ADW_APPLICATION_WINDOW(adw_application_window_new (GTK_APPLICATION (app)));
    gtk_window_set_title (GTK_WINDOW (win), app_name ? app_name : "Rhodes");
    gtk_window_set_default_size (GTK_WINDOW (win), 960, 640);
#ifdef RHO_ENABLE_CORE
    ALTGTK_LOG("Registering application window with core");
    rho_gtk_register_window(GTK_WINDOW(win));
#endif

    GtkWidget *root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_hexpand(root, TRUE);
    gtk_widget_set_vexpand(root, TRUE);
    adw_application_window_set_content(win, root);

    GtkWidget *menu_button = NULL;
    GtkWidget *menu_box = NULL;
    GtkWidget *header = altgtk_create_header(&menu_button, &menu_box);
    gtk_box_append(GTK_BOX(root), header);

    rho_gtk_register_menu_widgets(menu_button, menu_box);
#ifdef RHO_ENABLE_CORE
    rho_symimpl_createMenu();
#endif

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_vexpand(box, TRUE);
    gtk_box_append(GTK_BOX(root), box);

    gchar *splash_html = altgtk_locate_path(app_name, "rho/apps/app/loading.html", G_FILE_TEST_EXISTS);
    gchar *splash_uri = altgtk_path_to_uri(splash_html);

    ALTGTK_LOG("on_activate splash source=%s", splash_html ? splash_html : "(none)");

    #ifndef NO_WEBKIT
      GtkWidget *stack_widget = gtk_stack_new();
      gtk_widget_set_hexpand(stack_widget, TRUE);
      gtk_widget_set_vexpand(stack_widget, TRUE);
      gtk_box_append(GTK_BOX(box), stack_widget);
      g_content_stack = GTK_STACK(stack_widget);
      g_webview_widget = NULL;
      g_splash_widget = NULL;
      g_splash_visible = FALSE;

      WebKitWebView *view = WEBKIT_WEB_VIEW(webkit_web_view_new());
      WebKitSettings *settings = webkit_web_view_get_settings(view);
      if (settings) {
          webkit_settings_set_enable_javascript(settings, TRUE);
          webkit_settings_set_javascript_can_access_clipboard(settings, TRUE);
          webkit_settings_set_enable_developer_extras(settings, TRUE);
      }
      g_webview_widget = GTK_WIDGET(view);
      gtk_widget_set_hexpand(g_webview_widget, TRUE);
      gtk_widget_set_vexpand(g_webview_widget, TRUE);
      gtk_stack_add_named(g_content_stack, g_webview_widget, "webview");
      ALTGTK_LOG("WebKit view created, core=%s", ALTGTK_CORE_STATE_STRING);

      ALTGTK_LOG("Connecting load-changed handler");
      g_signal_connect(view, "load-changed", G_CALLBACK(gtk_on_load_changed), NULL);

      #ifdef RHO_ENABLE_CORE
        ALTGTK_LOG("Registering webview with core");
        rho_gtk_register_webview(view);
        ALTGTK_LOG("Installing native JS bridge");
        rho_gtk_install_native_bridge(view);
        g_signal_connect(view, "script-dialog", G_CALLBACK(gtk_on_script_dialog), NULL);
      #endif
        if (splash_uri && *splash_uri) {
            ALTGTK_LOG("Loading initial URI %s", splash_uri);
            webkit_web_view_load_uri(view, splash_uri);
#ifdef RHO_ENABLE_CORE
            rho_gtk_update_current_url(splash_uri);
#endif
            g_clear_pointer(&g_loading_html_uri, g_free);
            g_loading_html_uri = g_strdup(splash_uri);
        } else {
            ALTGTK_LOG("No splash HTML found, showing placeholder label");
            GtkWidget *lbl = gtk_label_new("RhoBundle HTML not found. Check installation.");
            gtk_widget_set_hexpand(lbl, TRUE);
            gtk_widget_set_vexpand(lbl, TRUE);
            gtk_stack_add_named(g_content_stack, lbl, "placeholder");
            gtk_stack_set_visible_child(g_content_stack, lbl);
            g_splash_widget = lbl;
            g_splash_visible = TRUE;
        }

        LoadCtx *ctx = g_new0(LoadCtx, 1);
        ctx->view = view;
        ctx->app_name = g_strdup(app_name);
        ALTGTK_LOG("Scheduling try_load_index in 1200 ms");
        g_timeout_add(1200, try_load_index, ctx);
    #ifdef RHO_ENABLE_CORE
        if (g_rho_core_started) {
            CoreLoadCtx *core_ctx = g_new0(CoreLoadCtx, 1);
            core_ctx->view = view;
            core_ctx->app_name = g_strdup(app_name);
            core_ctx->attempts = 0;
            ALTGTK_LOG("Scheduling try_load_core polling every 200 ms");
            g_timeout_add(200, try_load_core, core_ctx);
        }
    #endif

        gchar *splash_png = altgtk_locate_path(app_name, "rho/apps/app/loading.png", G_FILE_TEST_EXISTS);
        if (splash_png) {
            ALTGTK_LOG("Splash image source=%s", splash_png);
            GtkWidget *picture = gtk_picture_new_for_filename(splash_png);
            gtk_widget_set_hexpand(picture, TRUE);
            gtk_widget_set_vexpand(picture, TRUE);
            gtk_picture_set_content_fit(GTK_PICTURE(picture), GTK_CONTENT_FIT_CONTAIN);
            gtk_stack_add_named(g_content_stack, picture, "splash");
            gtk_stack_set_visible_child(g_content_stack, picture);
            g_splash_widget = picture;
            g_splash_visible = TRUE;
        } else if (g_webview_widget) {
            gtk_stack_set_visible_child(g_content_stack, g_webview_widget);
            g_splash_visible = FALSE;
            g_splash_widget = NULL;
        }
        g_clear_pointer(&splash_png, g_free);
    #else
        ALTGTK_LOG("WebKit not available, showing fallback label");
        GtkWidget *lbl = gtk_label_new("WebKitGTK not available. Install libwebkit2gtk4.1-devel and rebuild.");
        gtk_widget_set_hexpand(lbl, TRUE);
        gtk_widget_set_vexpand(lbl, TRUE);
        gtk_box_append(GTK_BOX(box), lbl);
    #endif

#ifdef RHO_ENABLE_CORE
    if (g_rho_core_started) {
        ALTGTK_LOG("Signaling core UI callbacks");
        rho_rhodesapp_callUiCreatedCallback();
        rho_rhodesapp_callAppActiveCallback(1);
    }
#endif

    g_clear_pointer(&splash_uri, g_free);
    g_clear_pointer(&splash_html, g_free);

    ALTGTK_LOG("Presenting application window");
    gtk_window_present(GTK_WINDOW(win));
}

// Process entry point: sanitizes env vars, boots GTK/Adwaita, and dispatches to g_application_run.
int
main (int argc, char *argv[])
{
#ifdef RHO_ENABLE_CORE
    altgtk_clear_ruby_environment();
#endif

    gchar *app_name = altgtk_derive_app_name(argc, argv);
    ALTGTK_LOG("Using application name %s", app_name);

    AdwApplication *app = ADW_APPLICATION (adw_application_new (ALTGTK_APP_ID, G_APPLICATION_DEFAULT_FLAGS));
#ifdef RHO_ENABLE_CORE
    ALTGTK_LOG("Connecting activate signal with core cleanup handler");
    g_signal_connect_data (app, "activate", G_CALLBACK (on_activate), g_strdup(app_name), on_activate_data_free, 0);
#else
    g_signal_connect_data (app, "activate", G_CALLBACK (on_activate), g_strdup(app_name), (GClosureNotify)g_free, 0);
#endif
    adw_init ();
    ALTGTK_LOG("GTK/Adwaita initialized");

#ifdef RHO_ENABLE_CORE
    altgtk_initialize_core(app_name);
#endif

    ALTGTK_LOG("Running GTK application loop");
    int status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    ALTGTK_LOG("Application exited with status %d", status);
    g_clear_pointer(&app_name, g_free);
    return status;
}
