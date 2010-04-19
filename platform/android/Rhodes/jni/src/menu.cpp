#include "JNIRhodes.h"

#include <jni/com_rhomobile_rhodes_RhoMenu.h>

#include <common/RhodesApp.h>
#include <common/AppMenu.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Menu"

typedef rho::Vector<rho::common::CAppMenu::Item> items_t;

static items_t &menu_items(jlong menu)
{
    return *reinterpret_cast<items_t*>(menu);
}

RHO_GLOBAL jlong JNICALL Java_com_rhomobile_rhodes_RhoMenu_allocMenu
  (JNIEnv *, jobject)
{
    std::auto_ptr<items_t> items(new items_t());
    if (!RHODESAPP().getAppMenu().getItems(*items))
        return 0;
    return reinterpret_cast<jlong>(items.release());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoMenu_deallocMenu
  (JNIEnv *, jobject, jlong menu)
{
    delete &menu_items(menu);
}

RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhodes_RhoMenu_getMenuSize
  (JNIEnv *, jobject, jlong menu)
{
    return menu_items(menu).size();
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhoMenu_getMenuItemLabel
  (JNIEnv *env, jobject, jlong menu, jint idx)
{
    return rho_cast<jstring>(env, menu_items(menu)[idx].getLabel().c_str());
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhoMenu_getMenuItemType
  (JNIEnv *env, jobject, jlong menu, jint idx)
{
    const char *s = "unknown";
    namespace r = rho::common;
    r::CAppMenu::Item &item = menu_items(menu)[idx];
    switch (item.getType())
    {
        case r::CAppMenu::Item::TYPE_URL: s = "url"; break;
        case r::CAppMenu::Item::TYPE_CMD_REFRESH: s = "refresh"; break;
        case r::CAppMenu::Item::TYPE_CMD_HOME: s = "home"; break;
        case r::CAppMenu::Item::TYPE_CMD_BACK: s = "back"; break;
        case r::CAppMenu::Item::TYPE_CMD_SYNC: s = "sync"; break;
        case r::CAppMenu::Item::TYPE_CMD_OPTIONS: s = "options"; break;
        case r::CAppMenu::Item::TYPE_CMD_LOG: s = "log"; break;
        case r::CAppMenu::Item::TYPE_CMD_EXIT: s = "exit"; break;
        case r::CAppMenu::Item::TYPE_CMD_CLOSE: s = "close"; break;
    }
    return rho_cast<jstring>(env, s);
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhoMenu_getMenuItemUrl
  (JNIEnv *env, jobject, jlong menu, jint idx)
{
    return rho_cast<jstring>(env, menu_items(menu)[idx].getLink().c_str());
}
