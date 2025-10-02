#include <gtk/gtk.h>

#include "common/RhoStd.h"
#include "api_generator/MethodResult.h"

#ifdef RHO_ENABLE_CORE
#include <string>
#include <vector>
#include <glib.h>

#include "logging/RhoLog.h"
#include "common/IRhoThreadImpl.h"
#include "common/RhodesApp.h"
#include "common/AppMenu.h"
#include "api_generator/MethodResult.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "GtkCore"

namespace {

struct GtkMenuState {
    GtkWidget *button = nullptr;
    GtkWidget *box = nullptr;
    std::vector<rho::common::CAppMenuItem> items;
    bool pending = false;
    guint update_source = 0;
};

static GtkMenuState s_menu_state;

static void menu_popdown() {
    if (!s_menu_state.button || !GTK_IS_MENU_BUTTON(s_menu_state.button))
        return;
    GtkPopover *popover = GTK_POPOVER(gtk_menu_button_get_popover(GTK_MENU_BUTTON(s_menu_state.button)));
    if (popover)
        gtk_popover_popdown(popover);
}

static void menu_clear_children(GtkWidget *container) {
    if (!GTK_IS_WIDGET(container))
        return;
    GtkWidget *child = gtk_widget_get_first_child(container);
    while (child) {
        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_widget_unparent(child);
        child = next;
    }
}

static void menu_update_button_state(bool has_items) {
    if (!s_menu_state.button)
        return;
    gtk_widget_set_visible(s_menu_state.button, has_items);
    gtk_widget_set_sensitive(s_menu_state.button, has_items);
}

static void menu_on_item_clicked(GtkButton *button, gpointer /*user_data*/) {
    gpointer index_ptr = g_object_get_data(G_OBJECT(button), "rho-menu-index");
    int index = GPOINTER_TO_INT(index_ptr);
    if (index < 0 || index >= static_cast<int>(s_menu_state.items.size()))
        return;
    rho::common::CAppMenuItem &item = s_menu_state.items[index];
    if (!item.m_isEnable)
        return;
    RAWLOG_INFO("Menu action triggered");
    item.processCommand();
    menu_popdown();
}

static gboolean menu_apply_update(gpointer /*data*/) {
    s_menu_state.update_source = 0;

    if (!s_menu_state.box || !GTK_IS_BOX(s_menu_state.box)) {
        menu_update_button_state(false);
        return G_SOURCE_REMOVE;
    }

    menu_clear_children(s_menu_state.box);
    bool has_items = false;

    for (size_t i = 0; i < s_menu_state.items.size(); ++i) {
        rho::common::CAppMenuItem &item = s_menu_state.items[i];
        if (item.m_eType == rho::common::CAppMenuItem::emtSeparator) {
            GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
            gtk_box_append(GTK_BOX(s_menu_state.box), sep);
            continue;
        }

        GtkWidget *btn = gtk_button_new_with_label(item.m_strLabel.c_str());
        gtk_button_set_has_frame(GTK_BUTTON(btn), FALSE);
        gtk_widget_set_halign(btn, GTK_ALIGN_FILL);
        gtk_widget_set_hexpand(btn, TRUE);
        gtk_widget_set_margin_top(btn, 2);
        gtk_widget_set_margin_bottom(btn, 2);
        gtk_widget_add_css_class(btn, "flat");
        g_object_set_data(G_OBJECT(btn), "rho-menu-index", GINT_TO_POINTER(static_cast<int>(i)));
        g_signal_connect(btn, "clicked", G_CALLBACK(menu_on_item_clicked), NULL);
        gtk_box_append(GTK_BOX(s_menu_state.box), btn);
        gtk_widget_set_sensitive(btn, item.m_isEnable);
        has_items = true;
    }

    if (!has_items)
        menu_popdown();

    menu_update_button_state(has_items);
    RAWLOG_INFO("Menu updated");
    return G_SOURCE_REMOVE;
}

static void schedule_menu_update() {
    if (s_menu_state.update_source != 0)
        return;
    s_menu_state.update_source = g_idle_add_full(G_PRIORITY_DEFAULT, menu_apply_update, NULL, NULL);
}

} // namespace

extern "C" {

void rho_webview_navigate(const char* url, int index);

struct GtkRunnableWrapper {
    rho::common::IRhoRunnable* task;
};

static gboolean gtk_execute_runnable(gpointer data) {
    GtkRunnableWrapper* wrapper = static_cast<GtkRunnableWrapper*>(data);
    if (wrapper) {
        if (wrapper->task) {
            wrapper->task->runObject();
            delete wrapper->task;
        }
        delete wrapper;
    }
    return G_SOURCE_REMOVE;
}

void rho_os_impl_performOnUiThread(rho::common::IRhoRunnable* pTask) {
    if (!pTask) {
        return;
    }
    GtkRunnableWrapper* wrapper = new GtkRunnableWrapper{pTask};
    g_idle_add_full(G_PRIORITY_DEFAULT, gtk_execute_runnable, wrapper, nullptr);
}

void* rho_nativethread_start() {
    RAWLOG_INFO("rho_nativethread_start() stub");
    return nullptr;
}

void rho_nativethread_end(void* /*data*/) {
    RAWLOG_INFO("rho_nativethread_end() stub");
}

void rho_net_impl_network_indicator(int /*active*/) {
    RAWLOG_INFO("rho_net_impl_network_indicator() stub");
}

int rho_net_ping_network(const char* /*host*/) {
    RAWLOG_INFO("rho_net_ping_network() stub");
    return 1;
}

void rho_map_location(char* query) {
    if (!query) return;
    std::string url = "https://maps.google.com/?";
    url += query;
    rho_webview_navigate(url.c_str(), 0);
}

void rho_appmanager_load(void* /*httpContext*/, char* /*query*/) {
    RAWLOG_INFO("rho_appmanager_load() stub");
}

void rho_sys_report_app_started() {
    RAWLOG_INFO("rho_sys_report_app_started() stub");
}

void notification_message_impl(const rho::Hashtable<rho::String, rho::String>& /*propertyMap*/) {
    RAWLOG_INFO("notification_message_impl() stub");
}

void notification_showStatusPopup(const rho::String& /*title*/, const rho::String& /*message*/, const rho::String& /*icon*/) {
    RAWLOG_INFO("notification_showStatusPopup() stub");
}

void notification_playFile_impl(const rho::String& /*fileName*/, const rho::Hashtable<rho::String, rho::String>& /*propertyMap*/) {
    RAWLOG_INFO("notification_playFile_impl() stub");
}

void notification_beep(const rho::Hashtable<rho::String, rho::String>& /*propertyMap*/) {
    RAWLOG_INFO("notification_beep() stub");
}

void alert_vibrate(const rho::Hashtable<rho::String, rho::String>& /*propertyMap*/) {
    RAWLOG_INFO("alert_vibrate() stub");
}

void alert_play_file(const rho::String& /*fileName*/, const rho::Hashtable<rho::String, rho::String>& /*propertyMap*/) {
    RAWLOG_INFO("alert_play_file() stub");
}

void alert_show_popup_ex(const rho::Hashtable<rho::String, rho::String>& /*propertyMap*/, rho::apiGenerator::CMethodResult& oResult) {
    RAWLOG_INFO("alert_show_popup_ex() stub");
    oResult.set(false);
}

void alert_show_status_ex(const rho::Hashtable<rho::String, rho::String>& /*propertyMap*/, rho::apiGenerator::CMethodResult& oResult) {
    RAWLOG_INFO("alert_show_status_ex() stub");
    oResult.set(false);
}

void alert_hide_popup() {
    RAWLOG_INFO("alert_hide_popup() stub");
}

void rho_gtk_register_menu_widgets(GtkWidget *button, GtkWidget *box) {
    s_menu_state.button = button;
    s_menu_state.box = box;
    RAWLOG_INFO("Menu widgets registered");
    if (!s_menu_state.items.empty() || s_menu_state.pending) {
        s_menu_state.pending = false;
        schedule_menu_update();
    } else {
        menu_update_button_state(false);
    }
}

void rho_symimpl_createMenu() {
    rho::Vector<rho::common::CAppMenuItem> items;
    RHODESAPP().getAppMenu().copyMenuItems(items);

    s_menu_state.items.clear();
    s_menu_state.items.reserve(items.size());
    for (size_t i = 0; i < items.size(); ++i) {
        s_menu_state.items.push_back(items[i]);
    }
    RAWLOG_INFO("rho_symimpl_createMenu invoked");

    if (!s_menu_state.box || !GTK_IS_BOX(s_menu_state.box)) {
        s_menu_state.pending = true;
        return;
    }

    s_menu_state.pending = false;
    schedule_menu_update();
}

} // extern "C"

#ifndef RHODES_VERSION_LIBRARY
bool rho_osximpl_toolbar_isStarted() {
    RAWLOG_INFO("rho_osximpl_toolbar_isStarted() stub");
    return false;
}

void rho_osximpl_toolbar_create(const rho::Vector<rho::String>& /*toolbarElements*/, const rho::Hashtable<rho::String, rho::String>& /*toolBarProperties*/) {
    RAWLOG_INFO("rho_osximpl_toolbar_create() stub");
}

void rho_osximpl_toolbar_remove() {
    RAWLOG_INFO("rho_osximpl_toolbar_remove() stub");
}

bool rho_osximpl_tabbar_isStarted() {
    RAWLOG_INFO("rho_osximpl_tabbar_isStarted() stub");
    return false;
}

void rho_osximpl_tabbar_create(const rho::Vector<rho::String>& /*tabbarElements*/, const rho::Hashtable<rho::String, rho::String>& /*tabBarProperties*/, rho::apiGenerator::CMethodResult& oResult) {
    RAWLOG_INFO("rho_osximpl_tabbar_create() stub");
    oResult.set(true);
}

void rho_osximpl_tabbar_remove() {
    RAWLOG_INFO("rho_osximpl_tabbar_remove() stub");
}

int rho_osximpl_tabbar_currentTabIndex() {
    RAWLOG_INFO("rho_osximpl_tabbar_currentTabIndex() stub");
    return 0;
}

void rho_osximpl_tabbar_switchTab(int /*tabIndex*/) {
    RAWLOG_INFO("rho_osximpl_tabbar_switchTab() stub");
}

void rho_osximpl_tabbar_setTabBadge(int /*tabIndex*/, const rho::String& /*badge*/) {
    RAWLOG_INFO("rho_osximpl_tabbar_setTabBadge() stub");
}

bool rho_wmimpl_toolbar_isStarted() {
    RAWLOG_INFO("rho_wmimpl_toolbar_isStarted() stub");
    return false;
}

void rho_wmimpl_toolbar_create(const rho::Vector<rho::String>& /*toolbarElements*/, const rho::Hashtable<rho::String, rho::String>& /*toolBarProperties*/) {
    RAWLOG_INFO("rho_wmimpl_toolbar_create() stub");
}

void rho_wmimpl_toolbar_remove() {
    RAWLOG_INFO("rho_wmimpl_toolbar_remove() stub");
}

bool rho_wmimpl_tabbar_isStarted() {
    RAWLOG_INFO("rho_wmimpl_tabbar_isStarted() stub");
    return false;
}

void rho_wmimpl_tabbar_create(const rho::Vector<rho::String>& /*tabbarElements*/, const rho::Hashtable<rho::String, rho::String>& /*tabBarProperties*/, rho::apiGenerator::CMethodResult& oResult) {
    RAWLOG_INFO("rho_wmimpl_tabbar_create() stub");
    oResult.set(true);
}

void rho_wmimpl_tabbar_remove() {
    RAWLOG_INFO("rho_wmimpl_tabbar_remove() stub");
}

void rho_wmimpl_tabbar_removeTab(int /*tabIndex*/) {
    RAWLOG_INFO("rho_wmimpl_tabbar_removeTab() stub");
}

int rho_wmimpl_tabbar_currentTabIndex() {
    RAWLOG_INFO("rho_wmimpl_tabbar_currentTabIndex() stub");
    return 0;
}

void rho_wmimpl_tabbar_switchTab(int /*tabIndex*/) {
    RAWLOG_INFO("rho_wmimpl_tabbar_switchTab() stub");
}

void rho_wmimpl_tabbar_setTabBadge(int /*tabIndex*/, const rho::String& /*badge*/) {
    RAWLOG_INFO("rho_wmimpl_tabbar_setTabBadge() stub");
}
#endif

#else

extern "C" {

void rho_gtk_register_menu_widgets(GtkWidget* /*button*/, GtkWidget* /*box*/) {}
void rho_symimpl_createMenu() {}
void* rho_nativethread_start() { return nullptr; }
void rho_nativethread_end(void* ) {}
void rho_net_impl_network_indicator(int ) {}
int rho_net_ping_network(const char* ) { return 1; }
void rho_map_location(char* ) {}
void rho_appmanager_load(void*, char*) {}
void rho_sys_report_app_started() {}
void notification_message_impl(const rho::Hashtable<rho::String, rho::String>& ) {}
void notification_showStatusPopup(const rho::String&, const rho::String&, const rho::String& ) {}
void notification_playFile_impl(const rho::String&, const rho::Hashtable<rho::String, rho::String>& ) {}
void notification_beep(const rho::Hashtable<rho::String, rho::String>& ) {}
void alert_vibrate(const rho::Hashtable<rho::String, rho::String>& ) {}
void alert_play_file(const rho::String&, const rho::Hashtable<rho::String, rho::String>& ) {}
void alert_show_popup_ex(const rho::Hashtable<rho::String, rho::String>&, rho::apiGenerator::CMethodResult& oResult) { oResult.set(false); }
void alert_show_status_ex(const rho::Hashtable<rho::String, rho::String>&, rho::apiGenerator::CMethodResult& oResult) { oResult.set(false); }
void alert_hide_popup() {}

bool rho_osximpl_toolbar_isStarted() { return false; }
void rho_osximpl_toolbar_create(const rho::Vector<rho::String>&, const rho::Hashtable<rho::String, rho::String>&) {}
void rho_osximpl_toolbar_remove() {}
bool rho_osximpl_tabbar_isStarted() { return false; }
void rho_osximpl_tabbar_create(const rho::Vector<rho::String>&, const rho::Hashtable<rho::String, rho::String>&, rho::apiGenerator::CMethodResult& oResult) { oResult.set(true); }
void rho_osximpl_tabbar_remove() {}
int rho_osximpl_tabbar_currentTabIndex() { return 0; }
void rho_osximpl_tabbar_switchTab(int) {}
void rho_osximpl_tabbar_setTabBadge(int, const rho::String&) {}
bool rho_wmimpl_toolbar_isStarted() { return false; }
void rho_wmimpl_toolbar_create(const rho::Vector<rho::String>&, const rho::Hashtable<rho::String, rho::String>&) {}
void rho_wmimpl_toolbar_remove() {}
bool rho_wmimpl_tabbar_isStarted() { return false; }
void rho_wmimpl_tabbar_create(const rho::Vector<rho::String>&, const rho::Hashtable<rho::String, rho::String>&, rho::apiGenerator::CMethodResult& oResult) { oResult.set(true); }
void rho_wmimpl_tabbar_remove() {}
void rho_wmimpl_tabbar_removeTab(int) {}
int rho_wmimpl_tabbar_currentTabIndex() { return 0; }
void rho_wmimpl_tabbar_switchTab(int) {}
void rho_wmimpl_tabbar_setTabBadge(int, const rho::String&) {}

} // extern "C"

#endif // RHO_ENABLE_CORE
