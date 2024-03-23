# Linux_power_notifyd
A simple light-weight power notification daemon for linux.<br>This software is aimed mainly at the minimal installs on laptops.<br>send a notification when :<br>
1. Battery is less at 15%<br>
2. When charger is connected/disconnected.<br>
# Dependencies
1. <a href="https://gnome.pages.gitlab.gnome.org/libnotify/">libnotify</a>
2. <a href="https://dunst-project.org/">dunst</a>
# Compilation
Compile main.c using<br>gcc -o <desired_name_of_compiled_program> ˋpkg-config --cflags --libs libnotifyˋ main.c<br>
# Usage
the usage of this software requires dunst to be running at all times this can be achieved by using `dunst&`<br>
furthurmore the compiled code can be run using `path/to/the/files/<name_of_compiled_code>&`<hr>
