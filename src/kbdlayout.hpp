/*============================================================================
Copyright (c) 2025 avarvit

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
============================================================================*/

#ifndef WIDGETS_KBDLAYOUT_HPP
#define WIDGETS_KBDLAYOUT_HPP

#include <widget.hpp>
#include <gtkmm/menubutton.h>
#include <gtkmm/label.h>
#include <giomm.h>

extern "C" {
#include "plugin.h"
}

#define PLUGIN_TITLE N_("Kbdlayout")
#define PLOGUN_DESC N_("Keybaord language display and switching")

class WidgetKbdLayout : public PanelWidget
{

    // a helper subclass of Gtk::MenuItem to ease up things with menus

    class MenuItem : public Gtk::MenuItem {

        std::string shrt_name;
        std::string long_name;
        WidgetKbdLayout *parent;

      public:
        MenuItem(WidgetKbdLayout *prnt, std::string shrtn, std::string longn, bool mnemonic = false) :
              Gtk::MenuItem(shrtn + " (" + longn + ")", mnemonic)
        {
           this->parent = prnt;
           this->shrt_name = shrtn;
           this->long_name = longn;
        }

        void set_activate()
        {
            this->signal_activate().connect_notify(
                sigc::mem_fun(this, &WidgetKbdLayout::MenuItem::on_clicked)
            );
        }

        std::string get_short_name()
        {
            return shrt_name;
        }

        std::string get_long_name()
        {
            return long_name;
        }

        void on_clicked()
        {
#if 0
            // debugging
            std::cerr << get_label()
                << " clicked (short name=" << get_short_name()
                << ", long name=" << get_long_name()
                << ")" << std::endl;
#endif
            parent->dbus_call_switch(get_short_name());
        }
    };

    // Menu stuff
    std::unique_ptr<Gtk::MenuButton> button;
    Gtk::Label label;
    Gtk::Menu menu;
    std::unordered_map<std::string,std::string> long_names;
    std::string get_long_name(std::string short_name);
    void reset_menu(const char *layout_list);


    // DBus stuff
    Glib::RefPtr<Glib::MainLoop> loop;
    Glib::RefPtr<Gio::DBus::Connection> connection;
    Glib::RefPtr<Gio::DBus::Proxy> proxy;
    void dbus_initialize();
    void dbus_call_enable(bool flag);
    void dbus_call_enable_off_on_pulse();
    void dbus_call_switch(std::string layout);
    void dbus_terminate();

    // Handlers
#if 0
    void on_button_press_event(GdkEventButton *event);
#endif

#if 0 // moved out, font is now a char * so conf_table can point to it
    // Options
    WfOption<std::string> bar_pos {"panel/position"};
    WfOption<std::string> font {"panel/kbdlayout_font"};
#endif

    char *font;
    void set_font();

#if 0 // moved to .cpp
    static constexpr conf_table_t conf_table[2] = {
        {CONF_STRING,   "font",     N_("Display font")},
        {CONF_NONE,     NULL,       NULL}
    };
#endif


  public:

#if 0
    // not used
    struct wl_seat *seat;
    struct wl_keyboard *keyboard;
#endif

    void widget_init(Gtk::HBox *container) override;
    void widget_config_reload(void) override;
    sigc::connection timeout;
    void widget_command(const char *cmd) override;
    void update_label(const char *layout);
    static const char *display_name (void) { return N_("KbdLayout"); };
#if 0 // moved to .cpp
    static const conf_table_t *config_params (void) { return conf_table; };
#endif
    virtual ~WidgetKbdLayout();

};

#endif /* end of include guard: WIDGETS_KBDLAYOUT_HPP */
