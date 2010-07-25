#!/usr/bin/perl -w

# $Id: install-SmoothInstall.pl 458 2010-03-03 22:28:13Z steve@domesticsecurity.com.au $

use lib "/usr/lib/smoothwall";
use SmoothInstall qw( :standard );
use Shell qw( cp cat );

# Set Up Environment
&Init("SmoothInstall");

my (@lines, $key, $pattern);

##################################################################################
PRINT $CYAN . "Patching bashrc $NORMAL";
##################################################################################
@lines = ('for i in `find /var/smoothwall/mods -maxdepth 1 -mindepth 1 -type d`;', 'do {', '        if [ -e "$i/bin" ]; then', '                if [[ ":$PATH:" == *":$i/bin:"* ]]; then', '                        return 0', '                fi', '                export PATH=$PATH:$i/bin', '        fi', '}', 'done');
$key = "si1";
$pattern = "locate";
&InsertAfter("/etc/bashrc", $key, $pattern, @lines);

my $TEST=`cat /etc/rc.d/rc.sysinit | grep "Starting Mod modprobe scripts"`;
if ($TEST) {
	$TEST=`cat /etc/rc.d/rc.sysinit | grep "Config::Patch-si1"`;
	if (!($TEST)) {
		PRINT $CYAN . "System Already Patched! $NORMAL";
		goto FINISH;
	}
}

##################################################################################
PRINT $CYAN . "Patching rc.sysinit (Startup Scripts) $NORMAL";
##################################################################################
@lines = ('echo "Starting Mod modprobe scripts"', 'for i in `find /var/smoothwall/mods -name 0[0-9]rc.sysinit | sort`; do $i; done');
$key = "si1";
$pattern = "Loading filesystems";
&InsertBefore("/etc/rc.d/rc.sysinit", $key, $pattern, @lines);

@lines = ('echo "Starting Mod run level scripts"', 'for i in `find /var/smoothwall/mods -name [1-9][0-9]rc.sysinit | sort`; do $i; done');
$key = "si2";
$pattern = "logger";
&InsertBefore("/etc/rc.d/rc.sysinit", $key, $pattern, @lines);

##################################################################################
PRINT $CYAN . "Patching crontab (cron.often) $NORMAL";
##################################################################################
@lines = ('*/5 * * * * root /usr/bin/smoothwall/run-parts `find /etc /var/smoothwall/mods -regex \'.*/etc/cron.often\'` >/dev/null');
$key = "si1";
$pattern = "cron.often";
&Change("/etc/crontab", $key, $pattern, @lines);

##################################################################################
PRINT $CYAN . "Patching crontab (cron.hourly) $NORMAL";
##################################################################################
@lines = ('01 * * * * root /usr/bin/smoothwall/run-parts `find /etc /var/smoothwall/mods -regex \'.*/etc/cron.hourly\'` >/dev/null');
$key = "si2";
$pattern = "cron.hourly";
&Change("/etc/crontab", $key, $pattern, @lines);

##################################################################################
PRINT $CYAN . "Patching crontab (cron.daily) $NORMAL";
##################################################################################
@lines = ('02 4 * * * root /usr/bin/smoothwall/run-parts `find /etc /var/smoothwall/mods -regex \'.*/etc/cron.daily\'` >/dev/null');
$key = "si3";
$pattern = "cron.daily";
&Change("/etc/crontab", $key, $pattern, @lines);

##################################################################################
PRINT $CYAN . "Patching crontab (cron.weekly) $NORMAL";
##################################################################################
@lines = ('22 4 * * 0 root /usr/bin/smoothwall/run-parts `find /etc /var/smoothwall/mods -regex \'.*/etc/cron.weekly\'` >/dev/null');
$key = "si4";
$pattern = "cron.weekly";
&Change("/etc/crontab", $key, $pattern, @lines);

##################################################################################
PRINT $CYAN . "Patching crontab (cron.monthly) $NORMAL";
##################################################################################
@lines = ('42 4 1 * * root /usr/bin/smoothwall/run-parts `find /etc /var/smoothwall/mods -regex \'.*/etc/cron.monthly\'` >/dev/null');
$key = "si5";
$pattern = "cron.monthly";
&Change("/etc/crontab", $key, $pattern, @lines);

##################################################################################
my $logrotate=`find /etc/cron.*/logrotate | head -n 1`;
chomp $logrotate;
PRINT $CYAN . "Patching logrotate ($logrotate) $NORMAL";
##################################################################################
@lines = ('echo "Rotating Mod Logs"', '/usr/sbin/logrotate `find /var/smoothwall/mods -regex \'.*/etc/logrotate.d\'`');
$key = "si1";
$pattern = "logrotate.conf";
&InsertAfter($logrotate, $key, $pattern, @lines);

##################################################################################
PRINT $CYAN . "Patching logrotate (/etc/rc.d/rc.sysinit) $NORMAL";
##################################################################################
@lines = ('echo "Rotating Mod Logs"', '/usr/sbin/logrotate `find /var/smoothwall/mods -regex \'.*/etc/logrotate.d\'`');
$key = "si3";
$pattern = "logrotate.conf";
&InsertAfter("/etc/rc.d/rc.sysinit", $key, $pattern, @lines);

##################################################################################
PRINT $CYAN . "Patching status.cgi $NORMAL";
##################################################################################
&Remove("/httpd/cgi-bin/status.cgi", "si1");
&Remove("/httpd/cgi-bin/status.cgi", "si2");
&Remove("/httpd/cgi-bin/status.cgi", "si3");
&Remove("/httpd/cgi-bin/status.cgi", "si4");
&Remove("/httpd/cgi-bin/status.cgi", "si5");
&Remove("/httpd/cgi-bin/status.cgi", "si6");
&Remove("/httpd/cgi-bin/status.cgi", "si7");

$key = "si1";
$pattern = 'opendir';
&CommentOut("/httpd/cgi-bin/status.cgi", $key, $pattern);

@lines = ('my @files = `find /usr/lib/smoothwall/services /var/smoothwall/mods -regex \'.*/usr/lib/smoothwall/services/.*\'`;');
$key = "si2";
$pattern = 'my @files';
&Change("/httpd/cgi-bin/status.cgi", $key, $pattern, @lines);

@lines = (
'        chomp($file);',
'        my $dirname  = dirname($file);',
'        my $basename = basename($file);'
);
$key = "si3";
$pattern = 'sort @files';
&InsertAfter("/httpd/cgi-bin/status.cgi", $key, $pattern, @lines);

@lines = ('        open ( my $line, "<$dirname/$basename" ) or next;');
$key = "si4";
$pattern = 'or next';
&Change("/httpd/cgi-bin/status.cgi", $key, $pattern, @lines);

@lines = ('        my $servicename = $basename;');
$key = "si5";
$pattern = '\$servicename = \$file';
&Change("/httpd/cgi-bin/status.cgi", $key, $pattern, @lines);

@lines = ('        if ($rel) {chomp $rel; };');
$key = "si6";
$pattern = 'chomp \$rel';
&Change("/httpd/cgi-bin/status.cgi", $key, $pattern, @lines);

@lines = ('use File::Basename;');
$key = "si7";
$pattern = 'use header';
&InsertAfter("/httpd/cgi-bin/status.cgi", $key, $pattern, @lines);


##################################################################################
PRINT $CYAN . "Patching httpd.conf $NORMAL";
##################################################################################
@lines = (
'ScriptAliasMatch ^/cgi-bin/mods/([^/]+)/(.*) /var/smoothwall/mods/$1/httpd/cgi-bin/$2', 
'<Directory "/var/smoothwall/mods/*/httpd/cgi-bin">',
'    AllowOverride AuthConfig',
'    Options ExecCGI',
'    Order allow,deny',
'    Allow from all',
'</Directory>',
'AliasMatch ^/mods/([^/]+)/(.*) /var/smoothwall/mods/$1/httpd/html/$2',
'<Directory "/var/smoothwall/mods/*/httpd/html">',
'    AllowOverride AuthConfig',
'    Options ExecCGI',
'    Order allow,deny',
'    Allow from all',
'</Directory>'
);
$key = "si1";
$pattern = '<Directory />';
&InsertBefore("/etc/httpd/conf/httpd.conf", $key, $pattern, @lines);

FINISH:

close STDERR;
open STDERR, ">/tmp/SmoothInstall.err";

PRINT $CYAN . "\nRestarting httpd $NORMAL";
@Args = ("-DSSL");
&StopMod("/usr/sbin/httpd", "httpd");
&StartMod("/usr/sbin/httpd", "httpd", @Args);

PRINT $CYAN . "\nRestarting smoothd $NORMAL";
@Args = ("");
&StopMod("/usr/sbin/smoothd", "smoothd");
&StartMod("/usr/sbin/smoothd", "smoothd", @Args);


PRINT $CYAN . "\nRestarting cron $NORMAL";
@Args = ("");
&StopMod("/usr/sbin/cron", "cron");
&StartMod("/usr/sbin/cron", "cron", @Args);

PRINT $CYAN . "\nUpdating CheckSI $NORMAL";
my @Files=`/usr/bin/find /var/smoothwall/mods/*/archives/CheckSI`;
foreach my $file (@Files) {
	cp("-f", "/var/smoothwall/mods/SmoothInstall/archives/CheckSI", "$file");
}



##################################################################################
PRINT $CYAN . "\nFinished Installing SmoothInstall. $NORMAL";
##################################################################################
system("/usr/bin/logger -t SmoothInstall \"SmoothInstall Version $ModDetails{'MOD_VERSION'} Installed\"");
unlink("/tmp/install-SmoothInstall");
