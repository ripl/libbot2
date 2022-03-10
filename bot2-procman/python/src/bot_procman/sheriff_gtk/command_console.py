# This file is part of bot2-procman.
#
# bot2-procman is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# bot2-procman is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with bot2-procman. If not, see
# <https://www.gnu.org/licenses/>.

import time

import gi
gi.require_version("Gtk", "3.0")

from gi.repository import Gdk
from gi.repository import GLib
from gi.repository import Gtk
from gi.repository import Pango

from bot_procman.printf_t import printf_t

DEFAULT_MAX_KB_PER_SECOND = 500

ANSI_CODES_TO_TEXT_TAG_PROPERTIES = {
    "1": ("weight", Pango.Weight.BOLD),
    "2": ("weight", Pango.Weight.LIGHT),
    "4": ("underline", Pango.Underline.SINGLE),
    "30": ("foreground", "black"),
    "31": ("foreground", "red"),
    "32": ("foreground", "green"),
    "33": ("foreground", "yellow"),
    "34": ("foreground", "blue"),
    "35": ("foreground", "magenta"),
    "36": ("foreground", "cyan"),
    "37": ("foreground", "white"),
    "40": ("background", "black"),
    "41": ("background", "red"),
    "42": ("background", "green"),
    "43": ("background", "yellow"),
    "44": ("background", "blue"),
    "45": ("background", "magenta"),
    "46": ("background", "cyan"),
    "47": ("background", "white"),
}


def now_str():
    return time.strftime("[%H:%M:%S] ")


class CommandExtraData(object):
    def __init__(self, text_tag_table):
        self.tb = Gtk.TextBuffer.new(text_tag_table)
        self.printf_keep_count = [0, 0, 0, 0, 0, 0]
        self.printf_drop_count = 0


class SheriffCommandConsole(Gtk.ScrolledWindow):
    def __init__(self, _sheriff, lc):
        super(SheriffCommandConsole, self).__init__()

        self.stdout_maxlines = 2000
        self.max_kb_per_sec = 0
        self.max_chars_per_2500_ms = 0

        self.sheriff = _sheriff

        # stdout textview
        self.stdout_textview = Gtk.TextView()
        self.stdout_textview.set_property("editable", False)
        self.sheriff_tb = self.stdout_textview.get_buffer()
        self.add(self.stdout_textview)

        stdout_adj = self.get_vadjustment()
        stdout_adj.scrolled_to_end = 1
        stdout_adj.connect("changed", self.on_adj_changed)
        stdout_adj.connect("value-changed", self.on_adj_value_changed)

        # add callback so we can add a clear option to the default right
        # click popup
        self.stdout_textview.connect("populate-popup",
                                     self.on_tb_populate_menu)

        # set some default appearance parameters
        self.font_str = "Monospace 10"
        self.set_font(self.font_str)
        self.base_color = Gdk.Color(65535, 65535, 65535)
        self.text_color = Gdk.Color(0, 0, 0)
        self.set_background_color(self.base_color)
        self.set_text_color(self.text_color)

        # stdout rate limit maintenance events
        GLib.timeout_add(500, self._stdout_rate_limit_upkeep)

        self.sheriff.command_added.connect(self._on_sheriff_command_added)
        self.sheriff.command_removed.connect(self._on_sheriff_command_removed)
        self.sheriff.command_status_changed.connect(
            self._on_sheriff_command_status_changed)

        self._cmd_extradata = {}

        lc.subscribe("PMD_PRINTF", self.on_procman_printf)

        self.text_tags = {"normal": Gtk.TextTag(name="normal")}
        for tt in self.text_tags.values():
            self.sheriff_tb.get_tag_table().add(tt)

        self.set_output_rate_limit(DEFAULT_MAX_KB_PER_SECOND)

    def get_background_color(self):
        return self.base_color

    def get_text_color(self):
        return self.text_color

    def get_font(self):
        return self.font_str

    def set_background_color(self, color):
        self.base_color = color
        self.stdout_textview.modify_base(Gtk.StateFlags.NORMAL, color)
        self.stdout_textview.modify_base(Gtk.StateFlags.ACTIVE, color)
        self.stdout_textview.modify_base(Gtk.StateFlags.PRELIGHT, color)

    def set_text_color(self, color):
        self.text_color = color
        self.stdout_textview.modify_text(Gtk.StateFlags.NORMAL, color)
        self.stdout_textview.modify_text(Gtk.StateFlags.ACTIVE, color)
        self.stdout_textview.modify_text(Gtk.StateFlags.PRELIGHT, color)

    def set_font(self, font_str):
        self.font_str = font_str
        self.stdout_textview.modify_font(Pango.FontDescription(font_str))

    def _stdout_rate_limit_upkeep(self):
        for cmd in self.sheriff.get_all_commands():
            extradata = self._cmd_extradata.get(cmd, None)
            if not extradata:
                continue
            if extradata.printf_drop_count:
                deputy = self.sheriff.get_command_deputy(cmd)
                self._add_text_to_buffer(
                    extradata.tb,
                    now_str()
                    + "\nSHERIFF RATE LIMIT: Ignored %d bytes of output\n" %
                    (extradata.printf_drop_count))
                self._add_text_to_buffer(
                    self.sheriff_tb,
                    now_str() + "Ignored %d bytes of output from [%s] [%s]\n" %
                    (extradata.printf_drop_count, deputy.name, cmd.command_id))

            extradata.printf_keep_count.pop(0)
            extradata.printf_keep_count.append(0)
            extradata.printf_drop_count = 0
        return True

    def _tag_from_seg(self, seg):
        esc_seq, seg = seg.split("m", 1)
        if not esc_seq:
            esc_seq = "0"
        key = esc_seq
        codes = esc_seq.split(";")
        if len(codes) > 0:
            codes.sort()
            key = ";".join(codes)
        if key not in self.text_tags:
            tag = Gtk.TextTag(name=key)
            for code in codes:
                if code in ANSI_CODES_TO_TEXT_TAG_PROPERTIES:
                    propname, propval = ANSI_CODES_TO_TEXT_TAG_PROPERTIES[code]
                    tag.set_property(propname, propval)
            self.sheriff_tb.get_tag_table().add(tag)
            self.text_tags[key] = tag
        return self.text_tags[key], seg

    def _add_text_to_buffer(self, tb, text):
        if not text:
            return

        # interpret text as ANSI escape sequences?  Try to format
        # colors...
        tag = self.text_tags["normal"]
        for segnum, seg in enumerate(text.split("\x1b[")):
            if not seg:
                continue
            if segnum > 0:
                try:
                    tag, seg = self._tag_from_seg(seg)
                except ValueError:
                    pass
            end_iter = tb.get_end_iter()
            tb.insert_with_tags(end_iter, seg, tag)

        # toss out old text if the muffer is getting too big
        num_lines = tb.get_line_count()
        if num_lines > self.stdout_maxlines:
            start_iter = tb.get_start_iter()
            chop_iter = tb.get_iter_at_line(num_lines - self.stdout_maxlines)
            tb.delete(start_iter, chop_iter)

    # Sheriff event handlers
    def _on_sheriff_command_added(self, deputy, command):
        extradata = CommandExtraData(self.sheriff_tb.get_tag_table())
        self._cmd_extradata[command] = extradata
        self._add_text_to_buffer(
            self.sheriff_tb,
            now_str() + "Added [%s] [%s] [%s]\n" %
            (deputy.name, command.command_id, command.exec_str))

    def _on_sheriff_command_removed(self, deputy, command):
        del self._cmd_extradata[command]
        self._add_text_to_buffer(
            self.sheriff_tb,
            now_str() + "[%s] removed [%s] [%s]\n" %
            (deputy.name, command.command_id, command.exec_str))

    def _on_sheriff_command_status_changed(self, cmd, old_status, new_status):
        self._add_text_to_buffer(
            self.sheriff_tb,
            now_str() + "[%s] new status: %s\n" % (cmd.command_id, new_status))

    def on_tb_populate_menu(self, textview, menu):
        sep = Gtk.SeparatorMenuItem()
        menu.append(sep)
        sep.show()
        mi = Gtk.MenuItem(label="_Clear")
        mi.set_use_underline(True)
        menu.append(mi)
        mi.connect("activate", self._tb_clear)
        mi.show()

    def _tb_clear(self, menu):
        tb = self.stdout_textview.get_buffer()
        start_iter = tb.get_start_iter()
        end_iter = tb.get_end_iter()
        tb.delete(start_iter, end_iter)

    def set_output_rate_limit(self, max_kb_per_sec):
        self.max_kb_per_sec = max_kb_per_sec
        self.max_chars_per_2500_ms = int(max_kb_per_sec * 1000 * 2.5)

    def get_output_rate_limit(self):
        return self.max_kb_per_sec

    def load_settings(self, save_map):
        if "console_rate_limit" in save_map:
            self.set_output_rate_limit(save_map["console_rate_limit"])

        if "console_background_color" in save_map:
            success, color = Gdk.Color.parse(
                save_map["console_background_color"])
            self.set_background_color(color)

        if "console_text_color" in save_map:
            success, color = Gdk.Color.parse(save_map["console_text_color"])
            self.set_text_color(color)

        if "console_font" in save_map:
            self.set_font(save_map["console_font"])

    def save_settings(self, save_map):
        save_map["console_rate_limit"] = self.max_kb_per_sec
        save_map["console_background_color"] = self.base_color.to_string()
        save_map["console_text_color"] = self.text_color.to_string()
        save_map["console_font"] = self.font_str

    def on_adj_changed(self, adj):
        if adj.scrolled_to_end:
            adj.set_value(adj.get_upper() - adj.get_page_size())

    def on_adj_value_changed(self, adj):
        adj.scrolled_to_end = adj.get_value(
        ) == adj.get_upper() - adj.get_page_size()

    def on_procman_printf(self, channel, data):
        msg = printf_t.decode(data)
        if msg.sheriff_id:
            try:
                cmd = self.sheriff.get_command_by_sheriff_id(msg.sheriff_id)
            except KeyError:
                # TODO
                return

            extradata = self._cmd_extradata.get(cmd, None)
            if not extradata:
                return

            # rate limit
            msg_count = sum(extradata.printf_keep_count)
            if msg_count >= self.max_chars_per_2500_ms:
                extradata.printf_drop_count += len(msg.text)
                return

            tokeep = min(self.max_chars_per_2500_ms - msg_count, len(msg.text))
            extradata.printf_keep_count[-1] += tokeep

            if len(msg.text) > tokeep:
                toadd = msg.text[:tokeep]
            else:
                toadd = msg.text

            self._add_text_to_buffer(extradata.tb, toadd)

    def show_command_buffer(self, cmd):
        extradata = self._cmd_extradata.get(cmd, None)
        if extradata:
            self.stdout_textview.set_buffer(extradata.tb)

    def show_sheriff_buffer(self):
        self.stdout_textview.set_buffer(self.sheriff_tb)
