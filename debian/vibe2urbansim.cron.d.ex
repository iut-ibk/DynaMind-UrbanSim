#
# Regular cron jobs for the vibe2urbansim package
#
0 4	* * *	root	[ -x /usr/bin/vibe2urbansim_maintenance ] && /usr/bin/vibe2urbansim_maintenance
