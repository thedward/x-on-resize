
This is a mirror of [git://people.freedesktop.org/~keithp/x-on-resize](git://people.freedesktop.org/~keithp/x-on-resize)

x-on-resize: a simple display configuration daemon
--------------------------------------------------

I like things to be automated as much as possible, and having abandoned Gnome to their own fate and switched to xfce, I missed the automatic display reconfiguration stuff. I decided to write something as simple as possible that did just what I needed. I did this a few months ago, and when Carl Worth asked what I was using, I decided to pack it up and make it available.

### Automatic configuration with a shell script

I've had a shell script around that I used to bind to a key press which I'd hit when I plugged or unplugged a monitor. So, all I really need to do is get this script run when something happens.

The missing tool here was something to wait for a change to happen and automatically invoke the script I'd already written.

### Resize vs Configure

The first version of x-on-resize just listened for ConfigureNotify events on the root window. These get sent every time anything happens with the screen configuration, from hot-plug to notification when someone runs xrandr. That was as simple as possible; the application was a few lines of code to select for ConfigureNotify events, and invoke a program provided on the command line.

However, it was a bit<span class="Apple-converted-space"> </span>*too*<span class="Apple-converted-space"> </span>simple as it would also respond to manual invocations of xrandr and call the script then as well. So, as long as I was content to accept whatever the script did, things were fine. And, with a laptop that had a DisplayPort connector for my external desktop monitor, and a separate VGA connector for projectors at conferences, the script always did something useful.

Then I got this silly laptop that has only DisplayPort, and for which a dongle is required to get to VGA for projectors. I probably could write something fancy to figure out the difference between a desktop DisplayPort monitor and DisplayPort to VGA dongle, but I decided that solving the simpler problem of only invoking the script on actual hotplug events would be better.

So, I left the current invoke-on-resize behavior intact and added new code that watched the list of available outputs and invoked a new 'config' script when that set changed.

The final program, x-on-resize, is available via git at

> git://people.freedesktop.org/~keithp/x-on-resize
