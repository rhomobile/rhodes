function wp7_change_forms_action_to_get() {
    for (i = 0; i < document.forms.length; i++) {
        document.forms[i].method = "GET";
    }
}
