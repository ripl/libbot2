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
from gi.repository import GObject
from gi.repository import Gtk

import bot_procman.sheriff as sheriff
import bot_procman.sheriff_gtk.command_model as cm
import bot_procman.sheriff_gtk.sheriff_dialogs as sd


class SheriffHostModel(Gtk.ListStore):
    COL_OBJ, COL_NAME, COL_LAST_UPDATE, COL_LOAD, NUM_ROWS = list(range(5))

    def __init__(self, _sheriff):
        super(SheriffHostModel, self).__init__(
            GObject.TYPE_PYOBJECT,
            GObject.TYPE_STRING,  # deputy name
            GObject.TYPE_STRING,  # last update time
            GObject.TYPE_STRING,  # load
        )
        self.sheriff = _sheriff

    def update(self):
        to_update = set(self.sheriff.get_deputies())
        to_remove = []

        def _deputy_last_update_str(dep):
            if dep.last_update_utime:
                now_utime = time.time() * 1000000
                return "%.1f seconds ago" % (
                    (now_utime - dep.last_update_utime) * 1e-6)
            else:
                return "<never>"

        def _update_host_row(model, path, model_iter, user_data):
            deputy = model.get_value(model_iter, SheriffHostModel.COL_OBJ)
            if deputy in to_update:
                model.set(
                    model_iter,
                    SheriffHostModel.COL_LAST_UPDATE,
                    _deputy_last_update_str(deputy),
                    SheriffHostModel.COL_LOAD,
                    "%f" % deputy.cpu_load,
                )
                to_update.remove(deputy)
            else:
                to_remove.append(Gtk.TreeRowReference(model, path))

        self.foreach(_update_host_row, None)

        for trr in to_remove:
            self.remove(self.get_iter(trr.get_path()))

        for deputy in to_update:
            new_row = (
                deputy,
                deputy.name,
                _deputy_last_update_str(deputy),
                "%f" % deputy.cpu_load,
            )
            self.append(new_row)


class SheriffHostTreeView(Gtk.TreeView):
    def __init__(self, _sheriff, hosts_ts):
        super(SheriffHostTreeView, self).__init__(hosts_ts)
        self.sheriff = _sheriff
        self.hosts_ts = hosts_ts

        plain_tr = Gtk.CellRendererText()
        col = Gtk.TreeViewColumn("Deputy",
                                 plain_tr,
                                 text=SheriffHostModel.COL_NAME)
        col.set_sort_column_id(1)
        col.set_resizable(True)
        self.append_column(col)

        last_update_tr = Gtk.CellRendererText()
        col = Gtk.TreeViewColumn("Last update",
                                 last_update_tr,
                                 text=SheriffHostModel.COL_LAST_UPDATE)
        col.set_resizable(True)
        col.set_cell_data_func(last_update_tr,
                               self._deputy_last_update_cell_data_func)
        self.append_column(col)

        col = Gtk.TreeViewColumn("Load",
                                 plain_tr,
                                 text=SheriffHostModel.COL_LOAD)
        col.set_resizable(True)
        self.append_column(col)

        self.connect("button-press-event",
                     self._on_hosts_tv_button_press_event)

        # hosts treeview context menu
        self.hosts_ctxt_menu = Gtk.Menu()

        self.cleanup_hosts_ctxt_mi = Gtk.MenuItem(label="_Cleanup")
        self.cleanup_hosts_ctxt_mi.set_use_underline(True)
        self.hosts_ctxt_menu.append(self.cleanup_hosts_ctxt_mi)
        self.cleanup_hosts_ctxt_mi.connect("activate", self._cleanup_hosts)
        self.hosts_ctxt_menu.show_all()

    def _on_hosts_tv_button_press_event(self, treeview, event):
        if event.type == Gdk.EventType.BUTTON_PRESS and event.button == 3:
            self.hosts_ctxt_menu.popup(None, None, None, None, event.button,
                                       event.time)
            return True

    def _cleanup_hosts(self, *args):
        self.sheriff.purge_useless_deputies()
        self.hosts_ts.update()

    def _deputy_last_update_cell_data_func(self, column, cell, model,
                                           model_iter, *data):
        # bit of a hack to pull out the last update time
        try:
            last_update = float(
                model.get_value(model_iter,
                                SheriffHostModel.COL_LAST_UPDATE).split()[0])
        except Exception:
            last_update = None
        if last_update is None or last_update > 5:
            cell.set_property("cell-background-set", True)
            cell.set_property("cell-background", "Red")
        elif last_update > 2:
            cell.set_property("cell-background-set", True)
            cell.set_property("cell-background", "Yellow")
        else:
            cell.set_property("cell-background-set", False)

    def save_settings(self, save_map):
        pass

    def load_settings(self, save_map):
        pass
